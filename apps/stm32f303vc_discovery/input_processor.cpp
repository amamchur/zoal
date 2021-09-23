#include "./input_processor.hpp"

#include "./board.hpp"
#include "./constants.hpp"
#include "./terminal.hpp"
#include "hardware.hpp"
#include "i2c.h"
#include "task.h"

#include <FreeRTOS.h>
#include <zoal/ic/lsm303dlhc.hpp>

using user_button_type = pcb::user_button_type<TickType_t>;
user_button_type user_button;
zoal::ic::lsm303dlhc<> lsm303dlhc;

[[noreturn]] void zoal_input_processor(void *) {
    user_button_type::pin::mode<zoal::gpio::pin_mode::input_floating>();

    using acc = zoal::ic::lsm303dlhc<>;
    constexpr auto reg1 = acc::ctrl_reg1_a_odr_400Hz | acc::ctrl_reg1_a_x_y_x_en;
    constexpr auto reg4 = acc::ctrl_reg4_a_bdu | acc::ctrl_reg4_a_fs_2g | acc::ctrl_reg4_a_hr;

    auto ctrl_reg4_a_initialized = [](int code){
        tx_stream << "ctrl_reg4_a_initialized: " << code << "\r\n";
        terminal.sync();
    };

    auto ctrl_reg1_a_initialized = [ctrl_reg4_a_initialized](int code){
        tx_stream << "ctrl_reg1_a_initialized: " << code << "\r\n";

        if (code == 0) {
            lsm303dlhc.config(i2c_dispatcher, acc::ctrl_reg4_a, reg4)(ctrl_reg4_a_initialized);
        }
    };

    auto button_callback = [](zoal::io::button_event e) {
        if (e == zoal::io::button_event::press) {
            lsm303dlhc.fetch(i2c_dispatcher)([](int code) {
                if (code == 0) {
                    auto v = lsm303dlhc.vector_sing();
                    auto dv = v.convert<double>();
                    dv /= (1 << 15);
                    dv *= 2; //+- 2g
                    tx_stream << "lsm303dlhc x: " << dv.x << " y: " << dv.y << " z: " << dv.z << " l: " << dv.length() << "\r\n";
                }
            });
        }
    };

    lsm303dlhc.config(i2c_dispatcher, acc::ctrl_reg1_a, reg1)(ctrl_reg1_a_initialized);

    while (true) {
        auto time = xTaskGetTickCount();
        user_button.handle(time, button_callback);
        vTaskDelay(1);
    }
}

#include "./input_processor.hpp"

#include "./command_queue.hpp"
#include "./terminal.hpp"
#include "task.h"

#include <FreeRTOS.h>

using user_button_type = pcb::user_button_type<TickType_t>;
user_button_type user_button;

[[noreturn]] void zoal_input_processor(void *) {
    user_button_type::pin::mode<zoal::gpio::pin_mode::input_floating>();

    using acc = zoal::ic::lsm303dlhc<>;
    constexpr auto reg1 = acc::ctrl_reg1_a_odr_400Hz | acc::ctrl_reg1_a_x_y_z_en;
    constexpr auto reg4 = acc::ctrl_reg4_a_bdu | acc::ctrl_reg4_a_fs_2g | acc::ctrl_reg4_a_hr;

    auto ctrl_reg4_a_initialized = [](int code) { terminal.sync(); };

    auto ctrl_reg1_a_initialized = [ctrl_reg4_a_initialized](int code) {
        if (code == 0) {
            lsm303dlhc.config(i2c_dispatcher, acc::ctrl_reg4_a, reg4)(ctrl_reg4_a_initialized);
        }
    };

    auto button_callback = [](zoal::io::button_event e) {
        if (e == zoal::io::button_event::press) {
            command_msg msg(app_cmd::axis);
            command_queue.push(msg);
        }
    };

    lsm303dlhc.config(i2c_dispatcher, acc::ctrl_reg1_a, reg1)(ctrl_reg1_a_initialized);

    while (true) {
        auto time = xTaskGetTickCount();
        user_button.handle(time, button_callback);
        vTaskDelay(1);
    }
}

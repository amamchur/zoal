#include "input_processor.hpp"

#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"

#include <FreeRTOS.h>

user_button_type user_button;

[[noreturn]] void zoal_input_processor(void *) {

    user_button_type::pin::mode<zoal::gpio::pin_mode::input_floating>();

    auto button_callback = [](zoal::io::button_event e) {
        if (e == zoal::io::button_event::press) {
            tx_stream << "Pressed\r\n";
        }
    };

    while (true) {
        auto time = xTaskGetTickCount();
        user_button.handle(time, button_callback);
        vTaskDelay(1);
    }
}

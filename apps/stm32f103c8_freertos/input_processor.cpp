#include "input_processor.hpp"

#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "types.hpp"

#include <FreeRTOS.h>

static user_button_1_type user_button_1;
static user_button_2_type user_button_2;

[[noreturn]] void zoal_input_processor(void *) {
    auto button_callback = [](zoal::io::button_event e, app_cmd cmd) {
        if (e == zoal::io::button_event::press) {
            command_queue.push(command_msg(cmd));
        }
    };

    while (true) {
        auto time = xTaskGetTickCount();
        user_button_1.handle(time, button_callback, app_cmd_button1_pressed);
        user_button_2.handle(time, button_callback, app_cmd_button2_pressed);
        vTaskDelay(5);
    }
}

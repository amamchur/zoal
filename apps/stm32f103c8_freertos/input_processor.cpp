#include "input_processor.hpp"

#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "types.hpp"

#include <FreeRTOS.h>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>

using row_selector = zoal::io::keypad_row_selector<mcu::pa_00, mcu::pa_01, mcu::pa_02, mcu::pa_03>;
using column_reader = zoal::io::keypad_column_reader<mcu::pa_04, mcu::pa_05, mcu::pa_06>;
using keypad_type = zoal::io::matrix_keypad<TickType_t, row_selector, column_reader>;

static keypad_type keypad;
static zoal::io::button<TickType_t, mcu::pb_12> user_button_1;
static zoal::io::button<TickType_t, mcu::pb_13> user_button_2;

[[noreturn]] void zoal_input_processor(void *) {
    auto button_callback = [](zoal::io::button_event e, app_cmd cmd) {
        if (e == zoal::io::button_event::press) {
            command_queue.push(command_msg(cmd));
        }
    };

    auto keypad_callback = [](zoal::io::button_event e, size_t r, size_t c) {
        if (e == zoal::io::button_event::press) {
            rx_stream_buffer.send(key_map[r][c]);
        }
    };

    while (true) {
        auto time = xTaskGetTickCount();
        user_button_1.handle(time, button_callback, app_cmd_button1_pressed);
        user_button_2.handle(time, button_callback, app_cmd_button2_pressed);
        keypad.handle(keypad_callback, time);
        vTaskDelay(5);
    }
}

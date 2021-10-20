#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "terminal.hpp"

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        command_msg msg;
        auto result = command_queue.pop(msg);
        if (!result || msg.command == app_cmd::none) {
            continue;
        }

        scoped_lock lock(tx_stream_mutex);
        tx_stream << "\033[2K\r";

        switch (msg.command) {
        case app_cmd::ticks:
            tx_stream << xTaskGetTickCount() << "\r\n";
            break;
        case app_cmd::help:
            tx_stream << help_message;
            break;
        default:
            break;
        }
        terminal.sync();
    }
}

#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "terminal.hpp"
#include "types.hpp"

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        app_cmd command = app_cmd_none;
        command_queue.pop(command);
        if (command == app_cmd_none) {
            continue;
        }

        tx_stream << "\r\n";
        switch (command) {
        case app_cmd_led_on:
            user_led::low();
            break;
        case app_cmd_led_off:
            user_led::high();
            break;
        case app_cmd_ticks:
            tx_stream << xTaskGetTickCount() << "\r\n";
            break;
        case app_cmd_button1_pressed:
            tx_stream << "button1_pressed\r\n";
            break;
        case app_cmd_button2_pressed:
            tx_stream << "button2_pressed\r\n";
            break;
        case app_cmd_not_found:
            tx_stream << "Not found\r\n";
            break;
        case app_cmd_print_heap_size:
            tx_stream << "free heap size: " << xPortGetFreeHeapSize() << "\r\n";
            break;
        case app_cmd_help:
            tx_stream << help_message;
            break;
        case app_cmd_task_info:
            tx_stream << "pcTaskName:\t\t" << taskStatus.pcTaskName << "\r\n";
            tx_stream << "eCurrentState:\t\t" << taskStatus.eCurrentState << "\r\n";
            tx_stream << "uxCurrentPriority:\t" << taskStatus.uxCurrentPriority << "\r\n";
            tx_stream << "uxBasePriority:\t\t" << taskStatus.uxBasePriority << "\r\n";
            tx_stream << "ulRunTimeCounter:\t" << taskStatus.ulRunTimeCounter << "\r\n";
            tx_stream << "pxStackBase:\t\t" << (void *)taskStatus.pxStackBase << "\r\n";
            tx_stream << "usStackHighWaterMark\t" << taskStatus.usStackHighWaterMark << "\r\n";
            break;
        default:
            break;
        }
        terminal.sync();
    }
}

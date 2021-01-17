#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "stm32f1xx_hal.h"
#include "task.h"
#include "terminal.hpp"
#include "types.hpp"
#include "rtc.h"

static inline void format_byte(char *ptr, uint8_t v) {
    ptr[0] = static_cast<char>(v % 10 + '0');
    ptr[-1] = static_cast<char>(v / 10 + '0');
}

void print_time() {
    RTC_TimeTypeDef time;
    char buffer[] = "00:00:00\r\n";

    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    format_byte(buffer + 1, time.Hours);
    format_byte(buffer + 4, time.Minutes);
    format_byte(buffer + 7, time.Seconds);
    tx_stream << buffer;
}

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        command_msg msg;
        command_queue.pop(msg);
        if (msg.command == app_cmd_none) {
            continue;
        }

        tx_stream << "\r\n";
        switch (msg.command) {
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
        case app_cmd_time_print:
            print_time();
            break;
        case app_cmd_time_set:
            HAL_RTC_SetTime(&hrtc, &msg.update_time.time, RTC_FORMAT_BIN);
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

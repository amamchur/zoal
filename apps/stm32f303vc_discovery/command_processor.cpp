#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "stm32f3xx_hal.h"
#include "task.h"
#include "terminal.hpp"
#include "types.hpp"

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        command_msg msg;
        command_queue.pop(msg);
        if (msg.command == app_cmd_none) {
            continue;
        }

        {
            __unused usart_debug_tx_transport::scoped_lock lock(tx_target.mutex);
            tx_stream << "\r\n";
            switch (msg.command) {
            case app_cmd_ticks:
                tx_stream << xTaskGetTickCount() << "\r\n";
                break;
            case app_cmd_print_heap_size:
                tx_stream << "free heap size: " << xPortGetFreeHeapSize() << "\r\n";
                break;
            case app_cmd_help:
                tx_stream << help_message;
                break;
            case app_cmd_task_info: {
                TaskHandle_t xTask = xTaskGetHandle(msg.task_name);
                if (xTask) {
                    TaskStatus_t taskStatus;
                    vTaskGetInfo(xTask, &taskStatus, pdTRUE, eInvalid);
                    tx_stream << "pcTaskName:\t\t" << taskStatus.pcTaskName << "\r\n";
                    tx_stream << "eCurrentState:\t\t" << taskStatus.eCurrentState << "\r\n";
                    tx_stream << "uxCurrentPriority:\t" << taskStatus.uxCurrentPriority << "\r\n";
                    tx_stream << "uxBasePriority:\t\t" << taskStatus.uxBasePriority << "\r\n";
                    tx_stream << "ulRunTimeCounter:\t" << taskStatus.ulRunTimeCounter << "\r\n";
                    tx_stream << "pxStackBase:\t\t" << (void *)taskStatus.pxStackBase << "\r\n";
                    tx_stream << "usStackHighWaterMark\t" << taskStatus.usStackHighWaterMark << "\r\n";
                } else {
                    tx_stream << "Task not found\r\n";
                }
                break;
            }
            default:
                tx_stream << "Not found\r\n";
                break;
            }
            terminal.sync();
        }
    }
}

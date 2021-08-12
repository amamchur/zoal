#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "terminal.hpp"

static uint8_t eeprom_mem[32];

static void read_eeprom() {
    eeprom.read(i2c_dispatcher, 0, eeprom_mem, 4)([](int code) {
        if (code == 0) {
            tx_stream << "\033[2K\r"
                      << "EEPROM:\r\n";
            for (int i = 0; i < 16; i++) {
                tx_stream << zoal::io::hexadecimal(eeprom_mem[i]) << "\r\n";
            }
        } else {
            tx_stream << "\033[2K\r"
                      << "Error"
                      << "\r\n";
        }
        terminal.sync();
    });
}

static void scan_i2c_devs() {
    scanner.device_found = [](uint8_t addr) {
        tx_stream << "\033[2K\r"
                  << "I2C Device found: " << zoal::io::hexadecimal(addr) << "\r\n";
    };
    scanner.scan(i2c_dispatcher)([](int code) {
        if (code == 0) {
            tx_stream << "\033[2K\r"
                      << "Done\r\n";
        } else {
            tx_stream << "\033[2K\r"
                      << "Error\r\n";
        }
        terminal.sync();
    });
}

static void read_rtc() {
    clock.fetch(i2c_dispatcher)([](int) {
        auto dt = clock.date_time();
        tx_stream << "\033[2K\r"
                  << "Date time: "
                  << "\r\n"
                  << dt << "\r\n";
        terminal.sync();
    });
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
            user_led::on();
            break;
        case app_cmd_led_off:
            user_led::off();
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
            read_rtc();
            break;
        case app_cmd_time_set:
            clock.date_time(msg.date_time);
            clock.update(i2c_dispatcher)([](int){});
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
        case app_cmd_scan_i2c:
            scan_i2c_devs();
            break;
        case app_cmd_read_eeprom:
            read_eeprom();
            break;
        default:
            break;
        }
        terminal.sync();
    }
}

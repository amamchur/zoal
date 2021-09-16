#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_flash_ex.h"
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
        scoped_lock lock(tx_stream_mutex);
        tx_stream << "\033[2K\r"
                  << "I2C Device found: " << zoal::io::hexadecimal(addr) << "\r\n";
    };
    scanner.scan(i2c_dispatcher)([](int code) {
        scoped_lock lock(tx_stream_mutex);
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
        scoped_lock lock(tx_stream_mutex);
        auto dt = clock.date_time();
        tx_stream << "\033[2K\r" << dt << "\r\n";
        terminal.sync();
    });
}

constexpr uintptr_t addr = 0x0801FC00;
auto test_value = reinterpret_cast<uint32_t *>(addr);

void flash_write() {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PAGEError;
    uint32_t DATA_32 = 0x12345678;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = addr;
    EraseInitStruct.NbPages = 1;
    auto result = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
    if (result == HAL_OK) {
        result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, DATA_32);
    }

    tx_stream << "Result: " << result << "\r\n";

    HAL_FLASH_Lock();
}

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
        case app_cmd::led:
            if (msg.int_value == 0) {
                user_led::off();
            } else {
                user_led::on();
            }
            break;
        case app_cmd::ticks:
            tx_stream << xTaskGetTickCount() << "\r\n";
            break;
        case app_cmd::button1_pressed:
            tx_stream << "button1_pressed\r\n";
            break;
        case app_cmd::button2_pressed:
            tx_stream << "button2_pressed\r\n";
            break;
        case app_cmd::print_heap_size:
            tx_stream << xPortGetFreeHeapSize() << "\r\n";
            break;
        case app_cmd::help:
            tx_stream << help_message;
            break;
        case app_cmd::time_print:
            read_rtc();
            break;
        case app_cmd::time_set:
            clock.date_time(msg.date_time_value);
            clock.update(i2c_dispatcher)([](int) {});
            break;
        case app_cmd::task_info: {
            TaskHandle_t xTask = xTaskGetHandle(msg.str_value);
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
        case app_cmd::scan_i2c:
            scan_i2c_devs();
            break;
        case app_cmd::read_eeprom:
            read_eeprom();
            break;
        default:
            break;
        }
        terminal.sync();
    }
}

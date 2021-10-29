#include "command_processor.hpp"

#include "FreeRTOS.h"
#include "adc.h"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "terminal.hpp"

#include <zoal/utils/i2c_scanner.hpp>

uint8_t spi_buffer[512];

zoal::utils::i2c_scanner scanner;

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
        __attribute__((unused)) scoped_lock lock(tx_stream_mutex);
        auto dt = clock.date_time();
        tx_stream << "\033[2K\r" << dt << "\r\n";
        tx_stream << clock.temperature() << "\r\n";
        terminal.sync();
    });
}

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
        case app_cmd::read_eeprom:
            read_eeprom();
            break;
        case app_cmd::time_print:
            read_rtc();
            break;
        case app_cmd::time_set:
            clock.date_time(msg.date_time_value);
            clock.update(i2c_dispatcher)([](int) {});
            break;
        case app_cmd::i2c:
            scan_i2c_devs();
            break;
        case app_cmd::pwm:
            pwm_channel::set(msg.int_values[0]);
            break;
        case app_cmd::pwm_on:
            pwm_channel::connect();
            break;
        case app_cmd::pwm_off:
            pwm_channel::disconnect();
            break;
        case app_cmd::timer:
            tx_stream << TIM2->CNT << "\r\n";
            break;
        case app_cmd::adc: {
            sensor_channel::adc::enable();
            sensor_channel::connect();
            auto v = sensor_channel::read();
            tx_stream << "adc: " << v << "\r\n";

            //            HAL_ADC_Start(&hadc1);
            //            HAL_ADC_PollForConversion(&hadc1,100);
            //            tx_stream << "adc: " << HAL_ADC_GetValue(&hadc1) << "\r\n";
            //            HAL_ADC_Stop(&hadc1);
            break;
        }
        case app_cmd::ticks:
            tx_stream << xTaskGetTickCount() << "\r\n";
            break;
        case app_cmd::w25q_write:
            for (int i = 0; i < 256; i++) {
                spi_buffer[i] = i;
            }
            w25q32::page_program(msg.int_values[0], spi_buffer, 256);
            break;
        case app_cmd::w25q_read:
            memset(spi_buffer, 0, sizeof(spi_buffer));
            w25q32::fast_read(msg.int_values[0], spi_buffer, sizeof(spi_buffer));
            for (int i = 0; i < sizeof(spi_buffer); ++i) {
                if ((i & 0xf) == 0) {
                    tx_stream << "\r\n";
                }
                tx_stream << zoal::io::hexadecimal(spi_buffer[i]) << " ";
            }
            tx_stream << "\r\n";
            break;
        case app_cmd::w25q_erase_chip:
            w25q32::chip_erase();
            break;
        case app_cmd::w25q_erase_sector:
            w25q32::sector_sector(msg.int_values[0]);
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

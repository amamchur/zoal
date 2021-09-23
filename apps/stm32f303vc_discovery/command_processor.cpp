#include "command_processor.hpp"

#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "task.h"
#include "terminal.hpp"

#include <zoal/utils/i2c_scanner.hpp>

zoal::utils::i2c_scanner scanner;

static void test_i2c() {
    scanner.device_found = [](uint8_t addr) {
        scoped_lock lock(tx_stream_mutex);
        tx_stream << "\033[2K\r"
                  << "I2C Device found: " << zoal::io::hexadecimal(addr) << "\r\n";
    };
    scanner.scan(i2c_dispatcher)([](int code) {
        scoped_lock lock(tx_stream_mutex);
        if (code == 0) {
            tx_stream << "Done\r\n";
        } else {
            tx_stream << "Error\r\n";
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

        {
            scoped_lock lock(tx_stream_mutex);
            tx_stream << "\033[2K\r";
            switch (msg.command) {
            case app_cmd::ticks:
                tx_stream << xTaskGetTickCount() << "\r\n";
                break;
            case app_cmd::led:
                if (msg.int_value == 0) {
                    ::pcb::led_03::off();
                } else {
                    ::pcb::led_03::on();
                }
                break;
            case app_cmd::scan_i2c:
                test_i2c();
                break;
            case app_cmd::help:
                tx_stream << help_message;
                break;
            default:
                tx_stream << "Not found\r\n";
                break;
            }
            terminal.sync();
        }
    }
}

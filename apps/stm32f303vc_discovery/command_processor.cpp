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

using leds_on = zoal::gpio::api::optimize<::pcb::led_03::on,
                                          ::pcb::led_04::on,
                                          ::pcb::led_05::on,
                                          ::pcb::led_06::on,
                                          ::pcb::led_07::on,
                                          ::pcb::led_08::on,
                                          ::pcb::led_09::on,
                                          ::pcb::led_10::on>;
using leds_off = zoal::gpio::api::optimize<::pcb::led_03::off,
                                           ::pcb::led_04::off,
                                           ::pcb::led_05::off,
                                           ::pcb::led_06::off,
                                           ::pcb::led_07::off,
                                           ::pcb::led_08::off,
                                           ::pcb::led_09::off,
                                           ::pcb::led_10::off>;

static void process_led(int l, int v) {
    switch (l) {
    case 3:
        if (v != 0) {
            ::pcb::led_03::on();
        } else {
            ::pcb::led_03::off();
        }
        break;
    case 4:
        if (v != 0) {
            ::pcb::led_04::on();
        } else {
            ::pcb::led_04::off();
        }
        break;
    case 5:
        if (v != 0) {
            ::pcb::led_05::on();
        } else {
            ::pcb::led_05::off();
        }
        break;
    case 6:
        if (v != 0) {
            ::pcb::led_06::on();
        } else {
            ::pcb::led_06::off();
        }
        break;
    case 7:
        if (v != 0) {
            ::pcb::led_07::on();
        } else {
            ::pcb::led_07::off();
        }
        break;
    case 8:
        if (v != 0) {
            ::pcb::led_08::on();
        } else {
            ::pcb::led_08::off();
        }
        break;
    case 9:
        if (v != 0) {
            ::pcb::led_09::on();
        } else {
            ::pcb::led_09::off();
        }
        break;
    case 10:
        if (v != 0) {
            ::pcb::led_10::on();
        } else {
            ::pcb::led_10::off();
        }
        break;
    default:
        break;
    }
}

static void read_acc_axis() {
    lsm303dlhc.fetch(i2c_dispatcher)([](int code) {
        if (code == 0) {
            scoped_lock lock(tx_stream_mutex);
            auto v = lsm303dlhc.vector_sing();
            auto dv = v.convert<double>();
            dv /= (1 << 15);
            dv *= 2; //+- 2g
            tx_stream << zoal::io::setprecision(5);
            tx_stream << "\033[2K\r";
            tx_stream << "x: " << dv.x << " y: " << dv.y << " z: " << dv.z << "\r\n";
            terminal.sync();
        }
    });
}

[[noreturn]] void zoal_cmd_processor(void *) {
    using zoal::gpio::api;

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
            case app_cmd::axis:
                read_acc_axis();
                break;
            case app_cmd::ticks:
                tx_stream << xTaskGetTickCount() << "\r\n";
                break;
            case app_cmd::led:
                process_led(msg.int_values[0], msg.int_values[1]);
                break;
            case app_cmd::leds:
                if (msg.int_values[0] != 0) {
                    leds_on();
                } else {
                    leds_off();
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

#ifndef ZOAL_IC_ADXL345_HPP
#define ZOAL_IC_ADXL345_HPP

#include "../data/vector.hpp"
#include "../periph/i2c_device.hpp"

#include <stdint.h>

namespace zoal { namespace ic {
    class adxl345 : public zoal::periph::i2c_device {
    public:
        adxl345() = default;
        explicit adxl345(uint8_t addr)
            : address_(addr) {}

        enum class register_map : uint8_t {
            device_id = 0x00,
            tap_threshold = 0x01D,
            x_axis_offset = 0x1E,
            y_axis_offset = 0x1F,
            z_axis_offset = 0x20,
            tap_duration = 0x21,
            tap_latency = 0x22,
            tap_window = 0x23,
            activity_threshold = 0x24,
            inactivity_threshold = 0x25,
            inactivity_time = 0x26,
            activity_control = 0x27,
            free_fall_threshold = 0x28,
            free_fall_time = 0x29,
            tap_control = 0x2A,
            tap_status = 0x2B,
            data_rate = 0x2C,
            power_control = 0x2D,
            interrupt_enable = 0x2E,
            interrupt_mapping = 0x2F,
            interrupt_source = 0x30,
            data_format = 0x31,
            x_axis_data_0 = 0x32,
            x_axis_data_1 = 0x33,
            y_axis_data_0 = 0x34,
            y_axis_data_1 = 0x35,
            z_axis_data_0 = 0x36,
            z_axis_data_1 = 0x37,
            fifo_control = 0x38,
            fifo_status = 0x37
        };

        template<class Dispatcher>
        typename Dispatcher::finisher_type power_on(Dispatcher &disp) {
            using zoal::periph::i2c_request_status;

            auto turn_on = [this](Dispatcher &dispatcher, zoal::periph::i2c_request_status s) {
                if (s == i2c_request_status::finished) {
                    buffer[0] = static_cast<uint8_t>(register_map::power_control);
                    buffer[1] = 8;

                    auto req = dispatcher.acquire_request();
                    req->write(address_, buffer, buffer + 2);
                    next_sequence(dispatcher, i2c_device::notify_client<Dispatcher>);
                } else {
                    dispatcher.finish_sequence(-1);
                }
            };

            // Turm off first
            buffer[0] = static_cast<uint8_t>(register_map::power_control);
            buffer[1] = 0;

            auto req = disp.acquire_request();
            req->write(address_, buffer, buffer + 2);
            next_sequence(disp, turn_on);
            return disp.make_finisher();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type fecth_axis(Dispatcher &disp) {
            using zoal::periph::i2c_request_status;

            auto read_axis = [this](Dispatcher &dispatcher, i2c_request_status s) {
                if (s == i2c_request_status::finished) {
                    auto req = dispatcher.acquire_request();
                    req->read(address_, buffer, buffer + 6);
                    next_sequence(dispatcher, notify_client<Dispatcher>);
                } else {
                    dispatcher.finish_sequence(-1);
                }
            };

            // Assign x_axis_data_0 address
            buffer[0] = static_cast<uint8_t>(register_map::x_axis_data_0);
            buffer[1] = 0;

            auto req = disp.acquire_request();
            req->write(address_, buffer, buffer + 1);
            next_sequence(disp, read_axis);
            return disp.make_finisher();
        }

        zoal::data::vector<int16_t> vector() {
            return zoal::data::vector<int16_t>(static_cast<int16_t>(buffer[1]) << 8 | buffer[0],
                                               static_cast<int16_t>(buffer[3]) << 8 | buffer[2],
                                               static_cast<int16_t>(buffer[5]) << 8 | buffer[4]);
        }

        uint8_t address_{0x53};
        uint8_t buffer[8]{0};
    };
}}

#endif

#ifndef ZOAL_IC_ADXL345_HPP
#define ZOAL_IC_ADXL345_HPP

#include "../data/vector.hpp"

#include <stdint.h>

namespace zoal { namespace ic {
    template<uint8_t Address = 0x53>
    class adxl345 {
    public:
        using self_type = adxl345<Address>;

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

        volatile bool ready_{false};
        volatile zoal::data::vector<int16_t> raw_vector;

        template<class Stream>
        uint8_t read_device_id(Stream *stream) {
            using i2c = typename Stream::i2c;
            ready_ = false;
            stream->callback = nullptr;
            stream->token = this;
            stream->write(Address).value(static_cast<uint8_t>(register_map::device_id));
            i2c::transmit(stream);
            i2c::wait();

            stream->read(Address, 1);
            i2c::transmit(stream);
            i2c::wait();

            return stream->data_[0];
        }

        template<class Stream>
        void read_axis(Stream *stream) {
            using i2c = typename Stream::i2c;
            ready_ = false;
            stream->callback = nullptr;
            stream->token = this;
            stream->write(Address).value(static_cast<uint8_t>(register_map::x_axis_data_0));
            i2c::transmit(stream);
            i2c::wait();

            stream->read(Address, 6);
            i2c::transmit(stream);
            i2c::wait();

            raw_vector.x = static_cast<int16_t >(stream->data_[1]) << 8 | stream->data_[0];
            raw_vector.y = static_cast<int16_t >(stream->data_[3]) << 8 | stream->data_[2];
            raw_vector.z = static_cast<int16_t >(stream->data_[5]) << 8 | stream->data_[4];
        }

        //        void fetch() {
        //            ready = false;
        //
        //            I2C::wait();
        //            I2C::stream().write(Address).value(static_cast<uint8_t>(register_map::device_id));
        //            I2C::transmit(&i2c_address_assigned, this);
        //        }

        static void i2c_address_assigned(void *token) {
            //            I2C::stream().read(Address, 1);
            //            I2C::transmit(&i2c_data_fetched, token);
        }

        static void i2c_data_fetched(void *token) {
            //            auto *cls = (self_type *) token;
            //            auto &stream = I2C::stream();
            //
            //            for (uint8_t i = 0; i < cls->data_size; i++) {
            //                cls->data[i] = stream.data[i];
            //            }
            //
            //            cls->ready_ = true;
        }
    };
}}

#endif

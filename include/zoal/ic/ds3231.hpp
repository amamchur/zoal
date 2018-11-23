#pragma once

#ifndef ZOAL_IC_DS3231_HPP
#define ZOAL_IC_DS3231_HPP

#include "../data/date_time.hpp"

namespace zoal { namespace ic {

    template<uint8_t Address = 0x68>
    class ds3231 {
    public:
        using self_type = ds3231<Address>;

        enum class reg_address : uint8_t {
            seconds = 0x00,
            minutes = 0x01,
            hours = 0x02,
            day = 0x03,
            date = 0x04,
            month_century = 0x05,
            year = 0x06,
            alarm1_seconds = 0x07,
            alarm1_minutes = 0x08,
            alarm1_hours = 0x09,
            alarm1_day_date = 0x0A,
            alarm2_minutes = 0x0B,
            alarm2_hours = 0x0C,
            alarm2_day_date = 0x0D,
            control = 0x0E,
            control_status = 0x0F,
            aging_offset = 0x10,
            temp_msb = 0x11,
            temp_lsb = 0x12
        };

        static constexpr uint8_t data_size = static_cast<uint8_t>(reg_address::temp_lsb) + 1;


//        template<class Stream>
//        friend class ds3231_i2c_callback;

        template<class Stream>
        class ds3231_i2c_callback {
        public:
            static void address_assigned(Stream *stream, void *token) {
                stream->read(Address, data_size);
                stream->callback = &ds3231_i2c_callback<Stream>::data_fetched;
                Stream::i2c::transmit(stream);
            }

            static void data_fetched(Stream *stream, void *token) {
                auto *obj = reinterpret_cast<self_type *>(token);
                for (uint8_t i = 0; i < stream->size(); i++) {
                    obj->data[i] = stream->data[i];
                }

                obj->ready = true;
            }

            static void i2c_data_updated(Stream *stream, void *token) {
                auto *obj = reinterpret_cast<self_type *>(token);
                obj->ready = true;
            }
        };

        template<class Stream>
        void fetch(Stream *stream) {
            ready = false;

            stream->callback = &ds3231_i2c_callback<Stream>::address_assigned;
            stream->token = this;
            stream->write(Address).value(static_cast<uint8_t>(reg_address::seconds));
            Stream::i2c::transmit(stream);
        }

        template<class Stream>
        void update(Stream *stream) {
            ready = false;

            stream->callback = &ds3231_i2c_callback<Stream>::i2c_data_updated;
            stream->token = this;
            stream->write(Address).value(static_cast<uint8_t>(reg_address::seconds));
            for (uint8_t i = 0; i < static_cast<uint8_t>(reg_address::temp_lsb); i++) {
                stream->value(data[i]);
            }

            Stream::i2c::transmit(stream);
        }

        zoal::data::date_time date_time() {
            auto &me = *this;
            zoal::data::date_time dt;
            dt.seconds = bcd2bin(me[reg_address::seconds]);
            dt.minutes = bcd2bin(me[reg_address::minutes]);
            dt.hours = bcd2bin(me[reg_address::hours]);

            dt.day = me[reg_address::day];
            dt.date = bcd2bin(me[reg_address::date]);
            dt.month = bcd2bin(me[reg_address::month_century]);
            dt.year = static_cast<uint16_t>(bcd2bin(me[reg_address::year]) + 2000);

            return dt;
        }

        void date_time(const zoal::data::date_time &dt) {
            auto &me = *this;
            me[reg_address::seconds] = bin2bcd(dt.seconds);
            me[reg_address::minutes] = bin2bcd(dt.minutes);
            me[reg_address::hours] = bin2bcd(dt.hours);
            me[reg_address::day] = dt.day;
            me[reg_address::date] = bin2bcd(dt.date);
            me[reg_address::month_century] = bin2bcd(dt.month);
            me[reg_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 2000));
        }

        float temperature() {
            uint8_t msb = data[reg_address::temp_msb];
            uint8_t lsb = data[reg_address::temp_lsb];
            return (float) msb + ((lsb >> 6u) * 0.25f);
        }

        uint8_t operator[](reg_address addr) const {
            return data[static_cast<uintptr_t>(addr)];
        }

        volatile uint8_t &operator[](reg_address addr) {
            return data[static_cast<uintptr_t>(addr)];
        }

        void wait() const {
            while (!ready);
        }

    private:
        volatile bool ready{false};

        volatile uint8_t data[data_size]{0};

        static uint8_t bcd2bin(uint8_t value) {
            return static_cast<uint8_t>(value - 6 * (value >> 4u));
        }

        static uint8_t bin2bcd(uint8_t value) {
            return static_cast<uint8_t>(value + 6 * (value / 10));
        }
    };
}}

#endif

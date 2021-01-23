#pragma once

#ifndef ZOAL_IC_DS3231_HPP
#define ZOAL_IC_DS3231_HPP

#include "../data/date_time.hpp"
#include "../periph/i2c_request.hpp"

namespace zoal { namespace ic {
    class ds3231 {
    public:
        static constexpr uint8_t century_flag = 1 << 7;
        static constexpr uint8_t month_mask = 0x1F;

        enum class register_address : uint8_t {
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

        enum ds3231_request {
            //
            unknown,
            reg_seconds_address_assigment_read,
            reg_data_fetch,
            reg_data_update
        };

        static constexpr uint8_t data_size = static_cast<uint8_t>(register_address::temp_lsb) + 1;

        ds3231() = default;
        explicit ds3231(uint8_t addr)
            : address_(addr) {}

        void fetch(zoal::periph::i2c_request &request) {
            request.initiator = this;
            request.token = reg_seconds_address_assigment_read;
            request.write(address_, &reg_addr, &reg_addr + sizeof(reg_addr));
        }

        void update(zoal::periph::i2c_request &request) {
            request.initiator = this;
            request.token = reg_data_update;
            request.write(address_, data_, data_ + sizeof(data_));
        }

        zoal::periph::i2c_request_completion_result complete_request(zoal::periph::i2c_request &request) {
            if (request.initiator != this || request.result != zoal::periph::i2c_result::ok) {
                return zoal::periph::i2c_request_completion_result::ignored;
            }

            switch (request.token) {
            case reg_seconds_address_assigment_read:
                request.token = reg_data_fetch;
                request.read(address_, data_, data_ + sizeof(data_));
                return zoal::periph::i2c_request_completion_result::new_request;
            case reg_data_fetch:
            case reg_data_update:
                return zoal::periph::i2c_request_completion_result::finished;
            default:
                return zoal::periph::i2c_request_completion_result::ignored;
            }
        }

        zoal::data::date_time date_time() const {
            auto &me = *this;
            zoal::data::date_time dt;
            dt.seconds = bcd2bin(me[register_address::seconds]);
            dt.minutes = bcd2bin(me[register_address::minutes]);
            dt.hours = bcd2bin(me[register_address::hours]);

            dt.day = (zoal::data::day_of_week)me[register_address::day];
            dt.date = bcd2bin(me[register_address::date]);
            dt.month = bcd2bin(me[register_address::month_century] & month_mask);
            dt.year = static_cast<uint16_t>(bcd2bin(me[register_address::year]) + 1900);
            if ((me[register_address::month_century] & century_flag) == century_flag) {
                dt.year += 100;
            }

            return dt;
        }

        void date_time(const zoal::data::date_time &dt) {
            auto &me = *this;
            me[register_address::seconds] = bin2bcd(dt.seconds);
            me[register_address::minutes] = bin2bcd(dt.minutes);
            me[register_address::hours] = bin2bcd(dt.hours);
            me[register_address::day] = dt.day;
            me[register_address::date] = bin2bcd(dt.date);
            me[register_address::month_century] = bin2bcd(dt.month) | (dt.year >= 2000 ? century_flag : 0);
            me[register_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 1900));
            if (dt.year > 2000) {
                me[register_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 2000));
            } else {
                me[register_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 1900));
            }
        }

        float temperature() {
            auto &me = *this;
            uint8_t msb = me[register_address::temp_msb];
            uint8_t lsb = me[register_address::temp_lsb];
            return static_cast<float>(msb) + static_cast<float>(lsb >> 6u) * 0.25f;
        }

        inline uint8_t operator[](register_address addr) const {
            return data_[static_cast<uintptr_t>(addr)];
        }

        inline uint8_t &operator[](register_address addr) {
            return data_[static_cast<uintptr_t>(addr)];
        }

    private:
        uint8_t address_{0x68};
        uint8_t reg_addr{static_cast<uint8_t>(register_address::seconds)};
        uint8_t data_[data_size]{0};

        static uint8_t bcd2bin(uint8_t value) {
            return static_cast<uint8_t>(value - 6 * (value >> 4u));
        }

        static uint8_t bin2bcd(uint8_t value) {
            return static_cast<uint8_t>(value + 6 * (value / 10));
        }
    };
}}

#endif

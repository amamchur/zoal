#pragma once

#ifndef ZOAL_IC_DS3231_HPP
#define ZOAL_IC_DS3231_HPP

#include "../data/date_time.hpp"
#include "../periph/i2c_device.hpp"
#include "../periph/i2c_request.hpp"

namespace zoal { namespace ic {
    class ds3231 : public zoal::periph::i2c_device {
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

        static constexpr uint8_t data_size = static_cast<uint8_t>(register_address::year) + 1;

        explicit ds3231(uint8_t addr = 0x68)
            : address_(addr) {}

        template<class Dispatcher>
        typename Dispatcher::finisher_type fetch(Dispatcher &disp) {
            auto notify_client = [](Dispatcher &dispatcher) { dispatcher.finish_sequence(); };

            auto address_assigned = [this, notify_client](Dispatcher &dispatcher) {
                auto req = dispatcher.acquire_request();
                auto size = static_cast<uint8_t>(register_address::year) - static_cast<uint8_t>(register_address::seconds);
                req->read(address_, this->data_, this->data_ + size);
                next_sequence(dispatcher, notify_client);
            };

            auto req = disp.acquire_request();
            req->write(address_, &reg_addr, &reg_addr + sizeof(reg_addr));
            next_sequence(disp, address_assigned);
            return disp.make_finisher();
        }

        template<class Dispatcher>
        typename Dispatcher::finisher_type update(Dispatcher &disp) {
            auto notify_client = [](Dispatcher &dispatcher) { dispatcher.finish_sequence(); };

            auto req = disp.acquire_request();
            req->write(address_, &reg_addr, &reg_addr + sizeof(data_));
            next_sequence(disp, notify_client);
            return disp.make_finisher();
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
            dt.year = static_cast<uint16_t>(bcd2bin(me[register_address::year]) + 2000);
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
            me[register_address::month_century] = bin2bcd(dt.month) | (dt.year >= 2100 ? century_flag : 0);
            me[register_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 2000));
            if (dt.year > 2100) {
                me[register_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 2100));
            } else {
                me[register_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 2000));
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
        uint8_t address_{0};
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

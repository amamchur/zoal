#pragma once

#ifndef ZOAL_IC_DS3231_HPP
#define ZOAL_IC_DS3231_HPP

#include "../data/date_time.hpp"

namespace zoal { namespace ic {

    template<class I2C, uint8_t Address = 0x68>
    class ds3231 {
    public:
        using self_type = ds3231<I2C, Address>;

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

        static constexpr uint8_t data_size = reg_address::temp_lsb + 1;

        volatile bool ready{false};

        void fetch() {
            ready = false;

            I2C::wait();
            I2C::stream().write(Address).value(reg_address::seconds);
            I2C::transmit(&i2c_address_assigned, this);
        }

        zoal::data::date_time date_time() {
            zoal::data::date_time dt;
            dt.seconds = bcd2bin(data[reg_address::seconds]);
            dt.minutes = bcd2bin(data[reg_address::minutes]);
            dt.hours = bcd2bin(data[reg_address::hours]);

            dt.day = data[reg_address::day];
            dt.date = bcd2bin(data[reg_address::date]);
            dt.month = bcd2bin(data[reg_address::month_century]);
            dt.year = bcd2bin(data[reg_address::year]) + 2000;

            return dt;
        }

        void date_time(const zoal::data::date_time &dt) {
            data[reg_address::seconds] = bin2bcd(dt.seconds);
            data[reg_address::minutes] = bin2bcd(dt.minutes);
            data[reg_address::hours] = bin2bcd(dt.hours);
            data[reg_address::day] = dt.day;
            data[reg_address::date] = bin2bcd(dt.date);
            data[reg_address::month_century] = bin2bcd(dt.month);
            data[reg_address::year] = bin2bcd(static_cast<uint8_t>(dt.year - 2000));
        }

        float temperature() {
            uint8_t msb = data[reg_address::temp_msb];
            uint8_t lsb = data[reg_address::temp_lsb];
            return (float) msb + ((lsb >> 6u) * 0.25f);
        }

        uint8_t operator[](uint8_t addr) const {
            return data[addr];
        }

        volatile uint8_t &operator[](uint8_t addr) {
            return data[addr];
        }

        void update() {
            I2C::wait();
            auto &stream = I2C::stream()
                    .write(Address)
                    .value(reg_address::seconds);
            for (uint8_t i = 0; i < reg_address::temp_lsb; i++) {
                stream.value(data[i]);
            }

            I2C::transmit(&i2c_data_updated, this);
        }

    private:
        volatile uint8_t data[data_size] = {0};
        uint32_t update_mask{0};

        static uint8_t bcd2bin(uint8_t value) {
            return static_cast<uint8_t>(value - 6 * (value >> 4u));
        }

        static uint8_t bin2bcd(uint8_t value) {
            return static_cast<uint8_t>(value + 6 * (value / 10));
        }

        static void i2c_data_updated(void *token) {
            auto *cls = (self_type *) token;
            cls->ready = true;
        }

        static void i2c_address_assigned(void *token) {
            I2C::stream().read(Address, data_size);
            I2C::transmit(&i2c_data_fetched, token);
        }

        static void i2c_data_fetched(void *token) {
            auto *cls = (self_type *) token;
            auto &stream = I2C::stream();

            for (uint8_t i = 0; i < data_size; i++) {
                cls->data[i] = stream.data[i];
            }

            cls->ready = true;
        }
    };
}}

#endif

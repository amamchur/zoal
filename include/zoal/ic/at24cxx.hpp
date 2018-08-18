#pragma once

#ifndef ZOAL_IC_AT24C32_HPP
#define ZOAL_IC_AT24C32_HPP

#include <stdint.h>

namespace zoal { namespace ic {
    template<class I2C, uint16_t PageSize, uint8_t Address = 0x57>
    class at24cxx {
    public:
        static constexpr uint16_t address = Address;
        static constexpr uint16_t page_size = PageSize;

        using self_type = at24cxx<I2C, PageSize, Address>;

        volatile bool ready{false};
        uint16_t page_address{0};
        uint16_t length{0};
        uint8_t page[PageSize];

        void read_page(uint16_t size = PageSize) {
            length = size;
            ready = false;
            I2C::wait();
            I2C::stream().write(Address)
                    .value(page_address >> 8)
                    .value(page_address & 0xFF);
            I2C::transmit(&i2c_address_assigned, this);
        }

        void write_page(uint16_t size = PageSize) {
            ready = false;
            length = size;
        }

        static void i2c_data_fetched(void *token) {
            // Class *cls = (Class *)token;
            // auto &stream = I2C::stream();

            // for (uint16_t i = 0; i < cls->length; i++) {
            // 	cls->page[i] = stream.data[i];
            // }

            // cls->ready = true;
        }


        static void i2c_address_assigned(void *token) {
            auto *cls = (self_type *) token;
            cls->ready = true;

            I2C::stream().read(Address, cls->length);
            // I2C::transmit(&i2cDataFetched, token);
        }
    };
}}

#endif

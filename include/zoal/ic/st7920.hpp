#pragma once

#ifndef ZOAL_IC_ST7920_HPP
#define ZOAL_IC_ST7920_HPP

#include <stdint.h>
#include "hd44780.hpp"
#include "../gpio/pin.hpp"

namespace zoal { namespace ic {
    template<class SPI, class CS, class Delay>
    class st7920_interface_spi {
    public:
        static constexpr uint8_t bit_mode = 8;

        using delay = Delay;
        using spi = typename SPI::MSBF_MODE_0;

        static void begin() {
            using namespace zoal::gpio;
            CS::template mode<pin_mode::output_push_pull>();
            CS::low();
            spi::begin();
            spi::setup();
        }

        static void send(uint8_t value, uint8_t rs) {
            CS::high();
            delay::microseconds(1);

            auto b0 = static_cast<uint8_t>(0xF8 | rs << 1);
            auto b1 = static_cast<uint8_t>(value & 0xF0);
            auto b2 = static_cast<uint8_t>((value & 0xF) << 4);
            spi::transfer(b0);
            spi::transfer(b1);
            spi::transfer(b2);

            CS::low();
        }
    };

    template<uint8_t Columns, uint8_t Rows>
    class st7920_address_selector {
    public:
        static constexpr uint8_t columns = Columns;
        static constexpr uint8_t rows = Rows;

        static uint8_t getAddress(uint8_t row, uint8_t column) {
            uint8_t address;
            switch (row) {
                case 1:
                    address = 0x10;
                    break;
                case 2:
                    address = 0x08;
                    break;
                case 3:
                    address = 0x18;
                    break;
                default:
                    address = 0x00;
                    break;
            }

            return address + column;
        }
    };

    template<class Interface, class AddressSelector>
    class st7920 : public hd44780<Interface, AddressSelector> {
    public:
        using base_type = hd44780<Interface, AddressSelector>;
        using delay = typename Interface::delay;

        template<uint8_t v = 0>
        class fill_iterator {
        public:
            fill_iterator &operator++() {
                return *this;
            }

            constexpr uint8_t operator*() {
                return v;
            }
        };

        st7920() = delete;

        static void extended_mode(bool on = true) {
            auto cfg = static_cast<uint8_t>(Interface::BitMode == 8 ? 0x10 : 0);
            cfg |= on ? 0x04 : 0x00;

            Interface::send(base_type::CmdFunctionSet | cfg, 0);
            delay::microseconds(37);
        }

        static void graphic_mode() {
            Interface::send(0x20, 0);
            delay::microseconds(37);

            Interface::send(0x24, 0);
            delay::microseconds(37);

            Interface::send(0x26, 0);
            delay::microseconds(37);
        }

        template<class Iter>
        static void screen_fill(Iter iter) {
            for (uint8_t i = 0; i < 64; i++) {
                auto a = static_cast<uint8_t>(i & 0x1F);
                auto b = static_cast<uint8_t>(i > 0x1F ? 8 : 0);
                base_type::ddram_addr(a);
                base_type::ddram_addr(b);

                for (uint8_t j = 0; j < 16; ++j, ++iter) {
                    this->write(*iter);
                }
            }
        }

        template<class Iter>
        static void sprite_fill(uint8_t x, uint8_t y, Iter iter) {
            for (uint8_t i = 0; i < 16; i++) {
                auto r = static_cast<uint8_t>(y * 16 + i);
                auto a = static_cast<uint8_t>(r & 0x1F);
                auto b = static_cast<uint8_t>((r > 0x1F ? 8 : 0) + x);
                base_type::ddram_addr(a);
                base_type::ddram_addr(b);

                for (uint8_t j = 0; j < 2; ++j, ++iter) {
                    base_type::write(*iter);
                }
            }
        }

        static void screen_clear() {
            screen_fill(fill_iterator<0>());
        }
    };
}}

#endif

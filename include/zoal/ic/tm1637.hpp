#pragma once

#ifndef ZOAL_IC_TM1637_HPP
#define ZOAL_IC_TM1637_HPP

#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"

namespace zoal { namespace ic {
    template<class Tools, class CLK, class DIO>
    class tm1637 {
    public:
        using delay = typename Tools::delay;

        class transaction {
        public:
            inline transaction() {
                CLK::high();
                DIO::high();
                DIO::low();
                CLK::low();
            }

            transaction(const transaction &) = delete;

            transaction &operator<<(uint8_t data) {
                using namespace zoal::gpio;

                for (uint8_t i = 0; i < 8; i++) {
                    CLK::low();
                    DIO::write(data & 0x01);
                    delay::ms(1);
                    CLK::high();
                    data = data >> 1;
                }

                CLK::low();
                DIO::high();
                CLK::high();

                DIO::template mode<pin_mode::input>();

                uint8_t tries = 0, ack;
                do {
                    ack = DIO::read();
                    if (++tries == 0) {
                        DIO::template mode<pin_mode::output>();
                        DIO::low();
                        DIO::template mode<pin_mode::input>();
                    }
                } while (ack);

                DIO::template mode<pin_mode::output>();
                return *this;
            }

            inline ~transaction() {
                CLK::low();
                DIO::low();
                CLK::high();
                DIO::high();
            }
        };

        enum : uint8_t {
            write_data = 0x40,
            set_address0 = 0xC0,
            set_address1 = 0xC1,
            set_address2 = 0xC2,
            set_address3 = 0xC3,
            set_address4 = 0xC4,
            set_address5 = 0xC5,
            display_off = 0x80,
            display_on = 0x88,
            pulse_width0 = display_on | 0,
            pulse_width1 = display_on | 1,
            pulse_width2 = display_on | 2,
            pulse_width3 = display_on | 3,
            pulse_width4 = display_on | 4,
            pulse_width5 = display_on | 5,
        };

        tm1637() = delete;

        static void begin() {
            using namespace zoal::gpio;
            zoal::gpio::api::template optimize<zoal::gpio::api::mode<pin_mode::output, DIO, CLK>>();
            //            api::template mode<pin_mode::output, DIO, CLK>::apply();
        }
    };
}}

#endif

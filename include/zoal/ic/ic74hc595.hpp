#pragma once

#ifndef ZOAL_IC74HC595_HPP
#define ZOAL_IC74HC595_HPP

#include "../gpio/pin_mode.hpp"
#include "../gpio/shift_out.hpp"

namespace zoal { namespace ic {
    template<class DS, class STCP, class SHCP>
    class ic74hc595 {
    public:
        typedef ic74hc595<DS, STCP, SHCP> self_type;

        class transaction {
        public:
            inline transaction() {
                STCP::high();
            }

            transaction(const transaction &) = delete;
            
            template<class T>
            transaction &operator<<(T data) {
                ::zoal::gpio::shift_out_msbf<DS, SHCP>(data);
                return *this;
            }

            inline ~transaction() {
                STCP::low();
            }
        };

        ic74hc595() = delete;

        static void init() {
            using namespace zoal::gpio;
            DS::template mode<pin_mode::output_push_pull>();
            STCP::template mode<pin_mode::output_push_pull>();
            SHCP::template mode<pin_mode::output_push_pull>();
        }

        static void write(uint8_t value) {
            if (value) {
                high();
            } else {
                low();
            }
        }

        static void low() {
            transaction t;
            DS::low();
            SHCP::low();
            SHCP::high();
        }

        static void high() {
            transaction t;
            DS::high();
            SHCP::low();
            SHCP::high();
        }
    };
}}

#endif

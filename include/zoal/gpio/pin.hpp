#pragma once

#ifndef ZOAL_GPIO_PIN_HPP
#define ZOAL_GPIO_PIN_HPP

#include "../utils/defs.hpp"
#include "null_port.hpp"
#include "pin_mode.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace gpio {

    template<class Port, uint8_t Offset, class... Mixin>
    class pin : public Mixin... {
    public:
        using port = Port;

        static constexpr bool not_pin = port::address == 0;
        static constexpr uint8_t offset = Offset;
        static constexpr typename port::register_type mask = not_pin ? 0 : 1u << Offset;

        template<::zoal::gpio::pin_mode PinMode>
        ZOAL_INLINE_IO static void mode() {
            port::template mode<PinMode, mask>();
        }

        ZOAL_INLINE_IO static uint8_t read() {
            return (port::read() >> offset) & 1u;
        }

        ZOAL_INLINE_IO static void write(uint8_t value) {
            if (value) {
                port::template high<mask>();
            } else {
                port::template low<mask>();
            }
        }

        template<uint8_t Value>
        ZOAL_INLINE_IO static void set() {
            if (Value) {
                port::template high<mask>();
            } else {
                port::template low<mask>();
            }
        }

        ZOAL_INLINE_IO static void low() {
            port::template low<mask>();
        }

        ZOAL_INLINE_IO static void high() {
            port::template high<mask>();
        }

        ZOAL_INLINE_IO static void toggle() {
            port::template toggle<mask>();
        }
    };

    class null_pin : public pin<null_port, 0> {};

    template<class Pin>
    class active_drain : public Pin {
    public:
        ZOAL_INLINE_IO static void on() {
            Pin::template mode<pin_mode::output>();
            Pin::low();
        }

        ZOAL_INLINE_IO static void off() {
            Pin::template mode<pin_mode::input_floating>();
        }
    };

    template<class Pin>
    class active_high : public Pin {
    public:
        ZOAL_INLINE_IO static void on() {
            Pin::template mode<pin_mode::output>();
            Pin::high();
        }

        ZOAL_INLINE_IO static void off() {
            Pin::low();
            Pin::template mode<pin_mode::output>();
        }
    };

    template<class Pin>
    class active_low : public Pin {
    public:
        ZOAL_INLINE_IO static void on() {
            Pin::template mode<pin_mode::output>();
            Pin::low();
        }

        ZOAL_INLINE_IO static void off() {
            Pin::template mode<pin_mode::output>();
            Pin::high();
        }
    };
}}

#endif

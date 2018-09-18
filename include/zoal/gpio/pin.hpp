#pragma once

#ifndef ZOAL_GPIO_PIN_HPP
#define ZOAL_GPIO_PIN_HPP

#include <stdint.h> /* NOLINT */

#include "pin_mode.hpp"
#include "null_port.hpp"

namespace zoal { namespace gpio {

    template<class Port, uint8_t Offset, class ... Mixin>
    class pin : public Mixin... {
    public:
        using port = Port;

        static constexpr bool not_pin = port::address == 0;
        static constexpr uint8_t offset = Offset;
        static constexpr typename port::register_type mask = not_pin ? 0 : 1u << Offset;

        uint8_t value;

        explicit pin(uint8_t &&v = 0) : value(static_cast<uint8_t>(v & 1u)) {}

        template<::zoal::gpio::pin_mode PinMode>
        static inline void mode() {
            typename port::template mode<PinMode, mask>();
        }

        static inline uint8_t read() {
            return (port::read() >> offset) & 1u;
        }

        static inline void write(uint8_t value) {
            if (value) {
                typename port::template high<mask>();
            } else {
                typename port::template low<mask>();
            }
        }

        template<uint8_t Value>
        static inline void set() {
            if (Value) {
                typename port::template high<mask>();
            } else {
                typename  port::template low<mask>();
            }
        }

        static inline void low() {
            typename port::template low<mask>();
        }

        static inline void high() {
            typename port::template high<mask>();
        }

        static inline void toggle() {
            typename port::template toggle<mask>();
        }
    };

    class null_pin : public pin<null_port, 0> {
    };

    template<class Pin>
    class active_drain : public Pin {
    public:
        explicit active_drain(uint8_t &&v = 1) : Pin(v & 1u) {}

        static inline void on() {
            Pin::template mode<pin_mode::output>();
            Pin::low();
        }

        static inline void off() {
            Pin::template mode<pin_mode::input_floating>();
        }
    };

    template<class Pin>
    class active_high : public Pin {
    public:
        explicit active_high(uint8_t &&v = 0) : Pin(v & 1u) {}

        static inline void on() {
            Pin::template mode<pin_mode::output>();
            Pin::high();
        }

        static inline void off() {
            Pin::low();
            Pin::template mode<pin_mode::output>();
        }
    };

    template<class Pin>
    class active_low : public Pin {
    public:
        explicit active_low(uint8_t &&v = 0) : Pin(v & 1u) {}

        static inline void on() {
            Pin::template mode<pin_mode::output>();
            Pin::low();
        }

        static inline void off() {
            Pin::template mode<pin_mode::output>();
            Pin::high();
        }
    };
}}

#endif

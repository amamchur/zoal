#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_CLOCK_SOURCE_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_CLOCK_SOURCE_HPP

#include <stdint.h>  /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    class prescaler_clock_source {
    public:
        static constexpr bool programmable_prescaler = false;
        static constexpr uint8_t prescaler_modes = 7;
        static constexpr uint8_t external_clocks = 0;

        template<uint8_t Index, class Dummy = void>
        struct prescaler_mode {
            static constexpr uint8_t index = 0xFF;
            static constexpr uint16_t value = 0;
            static constexpr uint8_t mask = 0x00;
            static_assert(Index < prescaler_modes, "Prescaler mode index is out of range");
        };
        template<class Dummy>
        struct prescaler_mode<0, Dummy> {
            static constexpr uint8_t index = 0x00;
            static constexpr uint16_t value = 0x0001;
            static constexpr uint8_t mask = 0x01;
        };
        template<class Dummy>
        struct prescaler_mode<1, Dummy> {
            static constexpr uint8_t index = 0x01;
            static constexpr uint16_t value = 0x0008;
            static constexpr uint8_t mask = 0x02;
        };
        template<class Dummy>
        struct prescaler_mode<2, Dummy> {
            static constexpr uint8_t index = 0x02;
            static constexpr uint16_t value = 0x0020;
            static constexpr uint8_t mask = 0x03;
        };
        template<class Dummy>
        struct prescaler_mode<3, Dummy> {
            static constexpr uint8_t index = 0x03;
            static constexpr uint16_t value = 0x0040;
            static constexpr uint8_t mask = 0x04;
        };
        template<class Dummy>
        struct prescaler_mode<4, Dummy> {
            static constexpr uint8_t index = 0x04;
            static constexpr uint16_t value = 0x0080;
            static constexpr uint8_t mask = 0x05;
        };
        template<class Dummy>
        struct prescaler_mode<5, Dummy> {
            static constexpr uint8_t index = 0x05;
            static constexpr uint16_t value = 0x0100;
            static constexpr uint8_t mask = 0x06;
        };
        template<class Dummy>
        struct prescaler_mode<6, Dummy> {
            static constexpr uint8_t index = 0x07;
            static constexpr uint16_t value = 0x0400;
            static constexpr uint8_t mask = 0x07;
        };

        template<uint8_t index, class Dummy = void>
        struct ExtClock {
            static constexpr uint16_t value = 0xFFFF;
            static constexpr uint8_t mask = 0x00;
            static_assert(index < external_clocks, "External clock index is out of range");
        };
    };

    class prescaler_and_ext_clock_source {
    public:
        static constexpr bool programmable_prescaler = false;
        static constexpr uint8_t prescaler_modes = 5;
        static constexpr uint8_t external_clocks = 2;

        template<uint8_t Index, class Dummy = void>
        struct prescaler_mode {
            static constexpr uint16_t value = 0;
            static constexpr uint8_t mask = 0x00;
            static_assert(Index < prescaler_modes, "Prescaler mode index is out of range");
        };
        template<class Dummy>
        struct prescaler_mode<0, Dummy> {
            static constexpr uint16_t value = 0x0001;
            static constexpr uint8_t mask = 0x01;
        };
        template<class Dummy>
        struct prescaler_mode<1, Dummy> {
            static constexpr uint16_t value = 0x0008;
            static constexpr uint8_t mask = 0x02;
        };
        template<class Dummy>
        struct prescaler_mode<2, Dummy> {
            static constexpr uint16_t value = 0x0040;
            static constexpr uint8_t mask = 0x03;
        };
        template<class Dummy>
        struct prescaler_mode<3, Dummy> {
            static constexpr uint16_t value = 0x0100;
            static constexpr uint8_t mask = 0x04;
        };
        template<class Dummy>
        struct prescaler_mode<4, Dummy> {
            static constexpr uint16_t value = 0x0400;
            static constexpr uint8_t mask = 0x05;
        };

        template<uint8_t index, class Dummy = void>
        struct ExtClock {
            static constexpr uint16_t value = 0xFFFF;
            static constexpr uint8_t mask = 0x00;
            static_assert(index < external_clocks, "External clock index is out of range");
        };
        template<class Dummy>
        struct ExtClock<0, Dummy> {
            static constexpr uint16_t value = 0x0000;
            static constexpr uint8_t mask = 0x06;
        };
        template<class Dummy>
        struct ExtClock<1, Dummy> {
            static constexpr uint16_t value = 0x0001;
            static constexpr uint8_t mask = 0x07;
        };
    };
}}}

#endif

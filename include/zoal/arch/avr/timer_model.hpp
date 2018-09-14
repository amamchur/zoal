
#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_MODEL_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_MODEL_HPP

#include <stdint.h> /* NOLINT */
#include "mcu_type.hpp"
#include "../../periph/timer_mode.hpp"
#include "../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<mcu_type Type>
    class timer8_mem_model {
    };

    template<>
    class timer8_mem_model<mcu_type::attiny13> {
    public:
        using word = uint8_t;

        static constexpr intptr_t OCRxB = 0;
        static constexpr intptr_t TCCRxA = 6;
        static constexpr intptr_t TCNTx = 9;
        static constexpr intptr_t TCCRxB = 10;
        static constexpr intptr_t OCRxA = 13;
    };

    template<>
    class timer8_mem_model<mcu_type::attiny2313> {
    public:
        using word = uint8_t;

        static constexpr intptr_t TCCRxA = 0;
        static constexpr intptr_t TCNTx = 2;
        static constexpr intptr_t TCCRxB = 3;
        static constexpr intptr_t OCRxA = 6;
        static constexpr intptr_t OCRxB = 12;
    };

    template<mcu_type type>
    class timer16_model {
    };

    template<>
    class timer16_model<mcu_type::attiny2313> {
    public:
        using word = uint16_t;

        static constexpr intptr_t ICRx = 0;
        static constexpr intptr_t OCRxB = 4;
        static constexpr intptr_t OCRxA = 6;
        static constexpr intptr_t TCNTx = 8;
        static constexpr intptr_t TCCRxB = 10;
        static constexpr intptr_t TCCRxA = 11;
    };
}}}

#endif

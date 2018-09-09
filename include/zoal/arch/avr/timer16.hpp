#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER16BIT_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER16BIT_HPP

#include <stdint.h>  /* NOLINT */
#include "timer.hpp"

namespace zoal { namespace arch { namespace avr {
    template<class Model, uintptr_t Address, uint8_t N>
    class timer16 : public timer<Model, Address, N> {
    public:
        static constexpr bool async = false;

        timer16() = delete;

        static typename Model::word top_value() {
            zoal::utils::memory_segment<uint8_t, Address> m8;
            auto wgm1 = m8[Model::TCCRxA] & 0x03;
            auto wgm2 = m8[Model::TCCRxB] & 0x18;
            auto wgm = wgm2 >> 1 | wgm1;

            switch (wgm) {
                case 0x01:
                case 0x05:
                    return 0x0FF;
                case 0x02:
                case 0x06:
                    return 0x1FF;
                case 0x03:
                case 0x07:
                    return 0x3FF;
                default:
                    return 0xFFFF;
            }
        }
    };
}}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class timer16_model {
    public:
        using word = uint16_t;

        static constexpr uintptr_t TCCRxA = 0;
        static constexpr uintptr_t TCCRxB = 1;
        static constexpr uintptr_t TCCRxC = 2;

        static constexpr uintptr_t TCNTx = 4;
        static constexpr uintptr_t ICRx = 6;
        static constexpr uintptr_t OCRxA = 8;
        static constexpr uintptr_t OCRxB = 10;
    };

    template<uintptr_t Address, uint8_t N>
    using timer16 = ::zoal::arch::avr::timer16<timer16_model, Address, N>;

}}}}

#endif

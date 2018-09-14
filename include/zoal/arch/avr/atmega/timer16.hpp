#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER16_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER16_HPP

#include "../timer16.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class timer16_model {
    public:
        using word = uint16_t;

        static constexpr uintptr_t TCCRxA = 0x00;
        static constexpr uintptr_t TCCRxB = 0x01;
        static constexpr uintptr_t TCCRxC = 0x02;
        static constexpr uintptr_t TCNTx = 0x04;
        static constexpr uintptr_t ICRx = 0x06;
        static constexpr uintptr_t OCRxA = 0x08;
        static constexpr uintptr_t OCRxB = 0x0A;

        static constexpr uint8_t TCCRxA_COMxA = 0xC0;
        static constexpr uint8_t TCCRxA_COMxB = 0x30;
        static constexpr uint8_t TCCRxA_WGMx = 0x03;
        static constexpr uint8_t TCCRxB_ICNCx = 0x80;
        static constexpr uint8_t TCCRxB_ICESx = 0x40;
        static constexpr uint8_t TCCRxB_WGMx = 0x18;
        static constexpr uint8_t TCCRxB_CSx = 0x07;
        static constexpr uint8_t TCCRxC_FOCxA = 0x80;
        static constexpr uint8_t TCCRxC_FOCxB = 0x40;
    };

    template<uintptr_t Address, uint8_t N>
    using timer16 = ::zoal::arch::avr::timer16<Address, N, timer16_model>;

}}}}

#endif

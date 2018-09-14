#ifndef ZOAL_TIMER16_HPP
#define ZOAL_TIMER16_HPP

#include <stdint.h>
#include "../timer16.hpp"

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

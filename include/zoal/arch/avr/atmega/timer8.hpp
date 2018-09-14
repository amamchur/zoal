#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER8_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER8_HPP

#include <stdint.h>
#include "../timer8.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class timer8_model {
    public:
        using word = uint8_t;

        static constexpr uintptr_t TCCRxA = 0;
        static constexpr uintptr_t TCCRxB = 1;
        static constexpr uintptr_t TCNTx = 2;
        static constexpr uintptr_t OCRxA = 3;
        static constexpr uintptr_t OCRxB = 4;
    };

    template<uintptr_t Address, uint8_t N, bool Async = false>
    using timer8 = ::zoal::arch::avr::timer8<Address, N, Async, timer8_model>;

}}}}

#endif

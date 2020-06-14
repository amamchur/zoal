#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER16BIT_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER16BIT_HPP

#include "timer.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, class... Mixin>
    class timer16 : public timer<Address, N, Mixin...> {
    public:
        static constexpr bool async = false;

        using self_type = timer<Address, N, Mixin...>;
        using word = typename self_type::word;

        static word period() {
            auto wgm1 = self_type::TCCRxA::ref() & 0x03;
            auto wgm2 = self_type::TCCRxB::ref() & 0x18;
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

#endif

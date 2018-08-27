#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP

#include <stdint.h>
#include "timer.hpp"

namespace zoal { namespace arch { namespace avr {
    template<class Model, uintptr_t Address, class TIFRs, class TIMRs, class ClkSrc, uint8_t N>
    class timer8 : public timer<Model, Address, TIFRs, TIMRs, ClkSrc, N> {
    public:
        using base_type = timer<Model, Address, TIFRs, TIMRs, ClkSrc, N>;

        timer8() = delete;

        static typename base_type::word top_value() {
            return 0xFF;
        }
    };
}}}

#endif

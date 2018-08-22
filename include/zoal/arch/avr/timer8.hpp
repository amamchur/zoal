#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP

#include <stdint.h>
#include "timer.hpp"

namespace zoal { namespace arch { namespace avr {
    template<class TimerModel, uintptr_t BaseAddress, class TIFRs, class TIMRs, class ClkSrc, uint8_t N>
    class timer8 : public timer<TimerModel, BaseAddress, TIFRs, TIMRs, ClkSrc, N> {
    public:
        using base_type = timer<TimerModel, BaseAddress, TIFRs, TIMRs, ClkSrc, N>;

        timer8() = delete;

        static typename base_type::word topValue() {
            return 0xFF;
        }
    };
}}}

#endif

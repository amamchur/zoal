#ifndef ZOAL_PERIPH_TIMER_HPP
#define ZOAL_PERIPH_TIMER_HPP

#include <stdint.h>

#include "timer_mode.hpp"

namespace zoal { namespace periph {
    template<uint32_t TimerFreq, uint32_t Prescaler, uint32_t Period, timer_mode Mode>
    class timer_config {
    public:
        static constexpr uint32_t prescaler = Prescaler;
        static constexpr uint32_t period = Period;
        static constexpr timer_mode mode = Mode;
    };
}}

#endif

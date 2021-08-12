#ifndef ZOAL_UTILS_MS_COUNTER_HPP
#define ZOAL_UTILS_MS_COUNTER_HPP

#include "defs.hpp"
#include "interrupts.hpp"

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace utils {
    template<typename T, volatile T *Pointer>
    class ms_counter {
    public:
        using value_type = T;
        static constexpr volatile T *value_ptr = Pointer;

        static volatile T now() {
            zoal::utils::interrupts_off off;
            return *value_ptr;
        }
    };

    template<uint32_t Frequency, uint16_t Prescale, uint16_t Period, uint16_t MicroSecPerTick = 1000>
    class timer_overflow_to_tick {
    public:
        static constexpr uint32_t timer_overflow_value = Period;

        static constexpr uint32_t clock_cycles_to_us(uint32_t clocks) {
            return (clocks * 1000L) / (Frequency / 1000L);
        }

        static constexpr uint16_t us_per_tick = MicroSecPerTick;
        static constexpr uint32_t us_per_overflow = clock_cycles_to_us(Prescale * timer_overflow_value);
        static constexpr uint16_t us_inc = static_cast<const uint16_t>(us_per_overflow / us_per_tick);
        static constexpr uint16_t fraction_inc = static_cast<const uint16_t>(us_per_overflow % us_per_tick);

        ZOAL_INLINE_IO static uint16_t step() {
            fraction_ += fraction_inc;
            if (fraction_ >= us_per_tick) {
                fraction_ -= us_per_tick;
                return 1 + us_inc;
            } else {
                return us_inc;
            }
        }

    private:
        static uint16_t fraction_;
    };

    template<uint32_t Frequency, uint16_t Prescale, uint16_t Period, uint16_t MicroSecPerTick>
    uint16_t timer_overflow_to_tick<Frequency, Prescale, Period, MicroSecPerTick>::fraction_ = 0;
}}

#endif

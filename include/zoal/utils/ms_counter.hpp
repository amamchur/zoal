#ifndef ZOAL_UTILS_MILLISECONDS_COUNTER_HPP
#define ZOAL_UTILS_MILLISECONDS_COUNTER_HPP

#include <stdint.h>
#include <stddef.h>
#include "volatile_value_reader.hpp"

namespace zoal { namespace utils {
    template<typename T, volatile T *Pointer>
    class ms_counter {
    public:
        typedef T value_type;
        static constexpr volatile T *value_ptr = Pointer;

        template<uint32_t Frequency, uint16_t prescale, class timer>
        class handler {
        private:
            static uint16_t fraction_;
        public:
            static constexpr uint32_t timer_overflow_value = (uint32_t) (1UL << timer::resolution);

            static constexpr uint32_t clock_cycles_to_us(uint32_t clocks) {
                return (clocks * 1000L) / (Frequency / 1000L);
            }

            static constexpr uint32_t us_per_overflow = clock_cycles_to_us(prescale * timer_overflow_value);
            static constexpr uint16_t us_inc = us_per_overflow / 1000;
            static constexpr uint16_t fraction_inc = static_cast<const uint16_t>(us_per_overflow % 1000);

            static inline void increment() {
                fraction_ += fraction_inc;
                if (fraction_ >= 1000) {
                    fraction_ -= 1000;
                    *value_ptr += 1 + us_inc;
                } else {
                    *value_ptr += us_inc;
                }
            }
        };

        static T now() {
            return volatile_value_reader<T, Pointer>();
        }
    };

    template<typename T, volatile T *Pointer>
    template<uint32_t Frequency, uint16_t Prescale, class timer>
    uint16_t ms_counter<T, Pointer>::handler<Frequency, Prescale, timer>::fraction_ = 0;

    class null_counter {
    public:
        typedef void value_type;
    };
}}

#endif

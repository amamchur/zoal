#ifndef ZOAL_UTILS_DELAY_HPP
#define ZOAL_UTILS_DELAY_HPP

#include <stdint.h>
#include "nop.hpp"

namespace zoal { namespace utils {
    template<class Microcontroller, class Counter>
    class delay {
    public:
        typedef Microcontroller mcu;
        typedef typename Counter::value_type counter_value_type;

        static void ms(counter_value_type milliseconds) {
            auto value = Counter::now() + milliseconds;
            while (Counter::now() < value);
        }

        template<uint64_t Milliseconds>
        static inline void ms() {
            constexpr uint64_t clocks = (mcu::frequency / 1000) * Milliseconds;
            nop<clocks>::place();
        }

        template<uint64_t Microseconds>
        static inline void us() {
            constexpr uint64_t clocks = (mcu::frequency / 1000000) * Microseconds;
            nop<clocks>::place();
        }

        template<uint64_t Nanoseconds>
        static inline void ns() {
            constexpr uint64_t clocks = (Nanoseconds * 1000) / mcu::ps_per_clock;
            nop<clocks>::place();
        }
    };
}}

#endif

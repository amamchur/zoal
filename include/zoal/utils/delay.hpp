#ifndef ZOAL_UTILS_DELAY_HPP
#define ZOAL_UTILS_DELAY_HPP

#include <stdint.h>
#include "nop.hpp"

namespace zoal { namespace utils {
    template<uint32_t SysFrequency, class Counter>
    class delay {
    public:
        using counter_value_type = typename Counter::value_type;
        static constexpr uint32_t frequency = SysFrequency;
        static constexpr uint32_t ps_per_clock  = (1000000000 / (frequency / 1000));

        static void ms(counter_value_type milliseconds) {
            volatile auto value = Counter::now() + milliseconds;
            while (Counter::now() < value);
        }

        template<uint64_t Milliseconds>
        static inline void ms() {
            constexpr uint64_t clocks = (frequency / 1000) * Milliseconds;
            nop<clocks>::place();
        }

        template<uint64_t Microseconds>
        static inline void us() {
            constexpr uint64_t clocks = (frequency / 1000000) * Microseconds;
            nop<clocks>::place();
        }

        template<uint64_t Nanoseconds>
        static inline void ns() {
            constexpr uint64_t clocks = (Nanoseconds * 1000) / ps_per_clock;
            nop<clocks>::place();
        }
    };
}}

#endif

#ifndef ZOAL_UTILS_DELAY_CMSIS_OS2_HPP
#define ZOAL_UTILS_DELAY_CMSIS_OS2_HPP

#include "cmsis_os.h"
#include "zoal/utils/nop.hpp"

#include <stdint.h>

namespace zoal { namespace utils { namespace cmsis_os2 {
    template<class Microcontroller>
    class delay {
    public:
        using mcu = Microcontroller;
        using counter_value_type = uint32_t;
        static constexpr uint32_t ps_per_clock  = (1000000000 / (mcu::frequency / 1000));

        static inline void ms(counter_value_type milliseconds) {
            osDelay(milliseconds);
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
            constexpr uint64_t clocks = (Nanoseconds * 1000) / ps_per_clock;
            nop<clocks>::place();
        }
    };
}}}

#endif

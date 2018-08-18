#ifndef ZOAL_ARCH_SIMULATOR_INTERRUPTS_HPP
#define ZOAL_ARCH_SIMULATOR_INTERRUPTS_HPP

#include <stdint.h>

namespace zoal { namespace utils {
    class interrupts {
    public:
        inline interrupts(bool) {
        }

        inline ~interrupts() {
        }

        static inline void off() {
        }

        static inline void on() {
        }
    };
}}

#endif

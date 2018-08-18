#ifndef ZOAL_ARCH_SIMULATOR_NOOPS_HPP
#define ZOAL_ARCH_SIMULATOR_NOOPS_HPP

#include <stdint.h>

namespace zoal { namespace utils {

    template<uint64_t count> struct nop {
        static inline void place() {
        }
    };
}}

#endif

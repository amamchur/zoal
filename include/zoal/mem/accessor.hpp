#ifndef ZOAL_MEM_ACCESSOR_HPP
#define ZOAL_MEM_ACCESSOR_HPP

#ifdef ZOAL_COVERAGE
#error "Direct memory accessor could not be used for unit tests"
#endif

#include "../utils/defs.hpp"

#include <stdint.h>

namespace zoal { namespace mem {
    template<class T, uintptr_t Address, uintptr_t Offset>
    struct accessor {
        static inline volatile T *ptr() {
            return reinterpret_cast<volatile T *>(Address + Offset);
        }

        static inline volatile T &ref() {
            return *reinterpret_cast<volatile T *>(Address + Offset);
        }
    };
}}

#endif

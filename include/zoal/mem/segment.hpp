#ifndef ZOAL_MEM_SEGMENT_HPP
#define ZOAL_MEM_SEGMENT_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace mem {
    template<class T, uintptr_t Address>
    class segment {
    public:
        inline __attribute__((always_inline)) volatile T &operator[](intptr_t offset) {
            return *reinterpret_cast<volatile T *>(reinterpret_cast<uint8_t *>(Address) + offset);
        }

        inline __attribute__((always_inline)) void happyInspection() const {
        }
    };
}}

#endif

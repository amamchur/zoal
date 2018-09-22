#ifndef ZOAL_MEM_SEGMENT_HPP
#define ZOAL_MEM_SEGMENT_HPP

#include "../utils/defs.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace mem {
    template<class T, uintptr_t Address>
    class segment {
    public:
        ZOAL_INLINE_IO volatile T &operator[](intptr_t offset) {
            return *reinterpret_cast<volatile T *>(reinterpret_cast<uint8_t *>(Address) + offset);
        }

        ZOAL_INLINE_IO void happyInspection() const {}

//        static volatile T *mem;
    };

//    template<class T, uintptr_t Address>
//    volatile T * segment<T, Address>::mem = reinterpret_cast<uint8_t *>(Address);
}}

#endif

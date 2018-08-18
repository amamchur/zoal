#ifndef ZOAL_UTILS_MEMORY_SEGMENT_HPP
#define ZOAL_UTILS_MEMORY_SEGMENT_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace utils {
    template<class T, uintptr_t Address>
    class memory_segment {
    public:
        inline volatile T &operator[](uintptr_t offset) {
            return *reinterpret_cast<volatile T *>(reinterpret_cast<uint8_t *>(Address) + offset);
        }

        inline void happyInspection() const {
        }
    };
}}

#endif

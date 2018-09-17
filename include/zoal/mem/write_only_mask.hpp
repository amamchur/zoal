#ifndef ZOAL_MEM_WRITE_ONLY_MASK_HPP
#define ZOAL_MEM_WRITE_ONLY_MASK_HPP

#include <stdint.h>

namespace zoal { namespace mem {
    template<uintptr_t Set, uint8_t Shift = 0>
    struct write_only_mask {
        static constexpr uint32_t mask = static_cast<uint32_t>(Set << Shift);

        template<class T>
        static inline void apply(T &value) {
            value = Set < Shift;
        }
    };

    template<uint8_t Shift>
    struct write_only_mask<0, Shift> {
        static constexpr uint32_t mask = 0;

        template<class T>
        static inline void apply(T &value) {}
    };
}}

#endif

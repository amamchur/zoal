#ifndef ZOAL_MEM_ACCESSOR_HPP
#define ZOAL_MEM_ACCESSOR_HPP

#include <stdint.h>

namespace zoal { namespace mem {
#if 0
    template <class T, uintptr_t Address, uintptr_t Offset>
    struct accessor {
        static constexpr volatile T *p = reinterpret_cast<volatile T *>(Address + Offset);
    };
#else
    template<class T, uintptr_t Address, uintptr_t Offset>
    struct accessor {
//        static volatile T *const p;

        static inline volatile T *ptr() {
            return reinterpret_cast<volatile T *>(Address + Offset);
        }

        static inline volatile T &ref() {
            return *reinterpret_cast<volatile T *>(Address + Offset);
        }
    };

//    template<class T, uintptr_t Address, uintptr_t Offset>
//    volatile T *const accessor<T, Address, Offset>::p = reinterpret_cast<volatile T *>(Address + Offset);
#endif
}}

#endif

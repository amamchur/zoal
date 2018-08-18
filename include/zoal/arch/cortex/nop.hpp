#ifndef ZOAL_ARCH_CORTEX_NOOP_CYCLES_HPP
#define ZOAL_ARCH_CORTEX_NOOP_CYCLES_HPP

#include <stdint.h>

namespace zoal { namespace utils {
    template<int32_t count>
    struct nop {
        static void place() {
            if (count < 0) {
                return;
            }

            nop<count & 0x001>::place();
            nop<count & 0x002>::place();
            nop<count & 0x004>::place();
            nop<count & 0x008>::place();
            nop<count & 0x010>::place();
            nop<count & 0x020>::place();
            nop<count & 0x040>::place();
            nop<count & 0x080>::place();
            nop<count & 0x100>::place();
            nop<count & 0x200>::place();
            nop<count & 0x400>::place();
        }
    };

    template<>
    struct nop<0x000> {
        static constexpr bool applicable = true;

        static inline void place() {
        }
    };

    template<>
    struct nop<0x001> {
        static constexpr bool applicable = true;

        static inline void place() {
            asm volatile("nop \n\t");
        }
    };

    template<>
    struct nop<0x002> {
        static constexpr bool applicable = true;

        static inline void place() {
            asm volatile(
            "nop \n\t"
            "nop \n\t"
            );
        }
    };

    template<>
    struct nop<0x004> {
        static constexpr bool applicable = true;

        static inline void place() {
            asm volatile(
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            );
        }
    };

    template<>
    struct nop<0x008> {
        static constexpr bool applicable = true;

        static inline void place() {
            asm volatile(
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            );
        }
    };

    template<>
    struct nop<0x010> {
        static constexpr bool applicable = true;

        static inline void place() {
            asm volatile(
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            "nop \n\t" "nop \n\t"
            );
        }
    };

    template<>
    struct nop<0x020> {
        static constexpr bool applicable = true;

        static void place() {
            nop<0x010>::place();
            nop<0x005>::place();
        }
    };

    template<>
    struct nop<0x040> {
        static constexpr bool applicable = true;

        static void place() {
            nop<0x020>::place();
            nop<0x010>::place();
        }
    };

    template<>
    struct nop<0x080> {
        static constexpr bool applicable = true;

        static void place() {
            nop<0x040>::place();
            nop<0x020>::place();
            nop<0x010>::place();
            nop<0x001>::place();
        }
    };

    template<>
    struct nop<0x100> {
        static constexpr bool applicable = true;

        static void place() {
            nop<0x080>::place();
            nop<0x040>::place();
            nop<0x020>::place();
            nop<0x010>::place();
            nop<0x004>::place();
            nop<0x002>::place();
        }
    };

    template<>
    struct nop<0x200> {
        static constexpr bool applicable = true;

        static void place() {
            nop<0x100>::place();
            nop<0x080>::place();
            nop<0x040>::place();
            nop<0x020>::place();
            nop<0x010>::place();
            nop<0x007>::place();
        }
    };

    template<>
    struct nop<0x400> {
        static constexpr bool applicable = true;

        static void place() {
            nop<0x200>::place();
            nop<0x100>::place();
            nop<0x080>::place();
            nop<0x040>::place();
            nop<0x020>::place();
            nop<0x010>::place();
        }
    };
}}

#endif

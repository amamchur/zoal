#ifndef ZOAL_ARCH_CORTEX_NOOP_CYCLES_HPP
#define ZOAL_ARCH_CORTEX_NOOP_CYCLES_HPP

#include <stdint.h>

namespace zoal { namespace utils {
    template<int32_t Count>
    struct nop;

    template<bool NoInlineLoop, int32_t Count>
    struct nop_strategy {};

    template<class Dummy>
    __attribute__((noinline)) void noinline_loop(uint32_t q) {
        asm volatile("1:	sub 	%[count],     #1		\n"
                     "      cmp     %[count],     #0      \n"
                     "   	bne     1b			    \n"
        :
        : [count] "r" (q)
        : "r0", "cc");
    }

    template<int32_t Count>
    struct nop_strategy<true, Count> {
        static constexpr int32_t call_overhead = 5;
        static constexpr int32_t loops = (Count - call_overhead) / 9;
        static constexpr int32_t rest = Count - (loops * 9) - call_overhead;

        static inline __attribute__((always_inline)) void place() {
            nop<rest>::place();
            noinline_loop<void>(static_cast<uint32_t >(loops));
        }
    };

    template<int32_t Count>
    struct nop : nop_strategy<(Count > 16), Count> {};

    template<>
    struct nop<0> {
        static inline __attribute__((always_inline)) void place() {}
    };

    template<>
    struct nop<1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n");
        }
    };

    template<>
    struct nop<2> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n nop \n");
        }
    };

    template<>
    struct nop<3> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n nop \n nop \n");
        }
    };

    template<>
    struct nop<4> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n nop \n nop \n nop \n");
        }
    };

    template<>
    struct nop<5> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n nop \n nop \n nop \n nop \n");
        }
    };

    template<>
    struct nop<6> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n");
        }
    };

    template<>
    struct nop<7> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n nop \n nop \n nop \n nop \n nop \n nop \n");
        }
    };

    template<>
    struct nop<8> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<9> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<10> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<11> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<12> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<13> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<14> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<15> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<16> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };
}}

#endif

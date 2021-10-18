#ifndef ZOAL_ARCH_CORTEX_NOOP_CYCLES_HPP
#define ZOAL_ARCH_CORTEX_NOOP_CYCLES_HPP

#include "./dwt.hpp"

#include <cstdint>

namespace zoal { namespace utils {
    template<int32_t Count>
    struct nop;

    template<int32_t Count, bool UseDataWatchpointTrigger>
    struct nop_stragegy {
    };

    template<int32_t Count>
    struct nop_stragegy<Count, true> {
        static constexpr uint32_t overhead = 32;
        static constexpr uint32_t cycles = Count - overhead;

        static inline __attribute__((always_inline)) void place() {
            zoal::arch::dwt::nop(cycles);
        }
    };

    template<int32_t Count>
    struct nop_stragegy<Count, false> {
        static constexpr uint32_t part_3 = (Count >= 48) ? 16 : 0;
        static constexpr uint32_t part_2 = (Count >= 32) ? 16 : 0;
        static constexpr uint32_t part_1 = (Count >= 16) ? 16 : 0;
        static constexpr uint32_t rest = Count % 16;

        static inline __attribute__((always_inline)) void place() {
            nop<part_3>::place();
            nop<part_2>::place();
            nop<part_1>::place();
            nop<rest>::place();
        }
    };

    template<int32_t Count>
    struct nop : public nop_stragegy<Count, (Count >= 64)> {
        nop() = delete;
    };

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
            asm volatile("nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<3> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<4> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<5> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<6> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<7> {
        static constexpr bool applicable = true;

        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
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
        static __attribute__((noinline)) void place() {
            asm volatile("nop \n");
        }
    };

    template<>
    struct nop<11> {
        nop() = delete;

        static __attribute__((noinline)) void place() {
            asm volatile("nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<12> {
        nop() = delete;

        static __attribute__((noinline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<13> {
        nop() = delete;

        static __attribute__((noinline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<14> {
        nop() = delete;

        static __attribute__((noinline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<15> {
        nop() = delete;

        static __attribute__((noinline)) void place() {
            asm volatile("nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n"
                         "nop \n");
        }
    };

    template<>
    struct nop<16> {
        nop() = delete;

        static __attribute__((noinline)) void place() {
            asm volatile("nop \n"
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

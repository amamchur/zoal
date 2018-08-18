#ifndef ZOAL_ARCH_ATMEL_AVR_NOOP_CYCLES_HPP
#define ZOAL_ARCH_ATMEL_AVR_NOOP_CYCLES_HPP

#include <stdint.h>
//
// Based on AVR delay Loop Calculator approach
// See http://www.bretmulvey.com/avrdelay.html
//
namespace zoal { namespace utils {
    template<uint64_t Count, uint64_t Depth>
    struct nop_depth {
        static_assert(true, "nop_depth should not be used directly");

        static inline __attribute__((always_inline)) void place() {
        }
    };

    template<>
    struct nop_depth<0, 1> {
        static inline __attribute__((always_inline)) void place() {
        }
    };

    template<>
    struct nop_depth<1, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("nop"            "\n\t");
        }
    };

    template<>
    struct nop_depth<2, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile("rjmp .+0"        "\n\t");
        }
    };

    template<>
    struct nop_depth<3, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "lpm"            "\n\t"
            );
        }
    };

    template<>
    struct nop_depth<4, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "rjmp .+0"        "\n\t"
            "rjmp .+0"        "\n\t"
            );
        }
    };

    template<>
    struct nop_depth<5, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "rjmp .+0"        "\n\t"
            "lpm"            "\n\t"
            );
        }
    };

    template<>
    struct nop_depth<6, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "lpm"            "\n\t"
            "lpm"            "\n\t"
            );
        }
    };

    template<>
    struct nop_depth<7, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "lpm"            "\n\t"
            "lpm"            "\n\t"
            "nop"            "\n\t"
            );
        }
    };

    template<>
    struct nop_depth<8, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "lpm"            "\n\t"
            "lpm"            "\n\t"
            "rjmp .+0"        "\n\t"
            );
        }
    };

    template<>
    struct nop_depth<9, 1> {
        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "lpm"            "\n\t"
            "lpm"            "\n\t"
            "lpm"            "\n\t"
            );
        }
    };

    template<uint64_t Depth>
    struct NopCalculator {
        static constexpr uint64_t min_value(uint64_t a, uint64_t b) {
            return a < b ? a : b;
        }

        static constexpr uint64_t max_value(uint64_t a, uint64_t b) {
            return a > b ? a : b;
        }

        static constexpr uint64_t limit_256(uint64_t value) {
            return max_value(1, min_value(256, value));
        }

        static constexpr uint64_t check_overflow(uint64_t value) {
            return value == 256 ? 0 : value;
        }

        static constexpr uint64_t cycles_in_loop(uint64_t d) {
            return d <= 1 ? 3 : (cycles_in_loop(d - 1) * 256 + 2);
        }

        static constexpr uint64_t branch_cycles(uint64_t loop) {
            return 3 * (Depth - loop);
        }

        static constexpr uint64_t loop_iterations(uint64_t cycles, uint64_t loop) {
            return check_overflow(limit_256((cycles - branch_cycles(loop)) / cycles_in_loop(Depth - loop) + 1));
        }

        static constexpr uint64_t rest_cycles(uint64_t cycles, uint64_t loop) {
            return cycles - (loop_iterations(cycles, loop) - 1) * cycles_in_loop(Depth - loop) - 3;
        }
    };

    template<uint64_t Count>
    struct nop_depth<Count, 1> : public NopCalculator<1> {
        static constexpr uint64_t loop0 = loop_iterations(Count, 0);
        static constexpr uint64_t rest0 = rest_cycles(Count, 0);

        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "		ldi  r18, %[loop0]"    "\n\t"
            "1:		dec  r18"            "\n\t"
            "		brne 1b"            "\n\t"
            :
            :[loop0] "i"(loop0)
            : "r18"
            );
        }
    };

    template<uint64_t Count>
    struct nop_depth<Count, 2> : public NopCalculator<2> {
        static constexpr uint64_t loop0 = loop_iterations(Count, 0);
        static constexpr uint64_t rest0 = rest_cycles(Count, 0);
        static constexpr uint64_t loop1 = loop_iterations(rest0, 1);
        static constexpr uint64_t rest1 = rest_cycles(rest0, 1);

        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "    ldi  r18, %[loop0]"    "\n"
            "    ldi  r19, %[loop1]"    "\n"
            "1:  dec  r19"                "\n"
            "    brne 1b"                "\n"
            "    dec  r18"                "\n"
            "    brne 1b"                "\n"
            :
            :[loop0] "i"(loop0),[loop1] "i"(loop1)
            : "r18", "r19"
            );
        }
    };

    template<uint64_t Count>
    struct nop_depth<Count, 3> : public NopCalculator<3> {
        static constexpr uint64_t loop0 = loop_iterations(Count, 0);
        static constexpr uint64_t rest0 = rest_cycles(Count, 0);
        static constexpr uint64_t loop1 = loop_iterations(rest0, 1);
        static constexpr uint64_t rest1 = rest_cycles(rest0, 1);
        static constexpr uint64_t loop2 = loop_iterations(rest1, 2);
        static constexpr uint64_t rest2 = rest_cycles(rest1, 2);

        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "    ldi  r18, %[loop0]"    "\n"
            "    ldi  r19, %[loop1]"    "\n"
            "    ldi  r20, %[loop2]"    "\n"
            "1:  dec  r20"                "\n"
            "    brne 1b"                "\n"
            "    dec  r19"                "\n"
            "    brne 1b"                "\n"
            "    dec  r18"                "\n"
            "    brne 1b"                "\n"
            :
            :[loop0] "i"(loop0),[loop1] "i"(loop1),[loop2] "i"(loop2)
            : "r18", "r19", "r20"
            );
        }
    };

    template<uint64_t Count>
    struct nop_depth<Count, 4> : public NopCalculator<4> {
        static constexpr uint64_t loop0 = loop_iterations(Count, 0);
        static constexpr uint64_t rest0 = rest_cycles(Count, 0);
        static constexpr uint64_t loop1 = loop_iterations(rest0, 1);
        static constexpr uint64_t rest1 = rest_cycles(rest0, 1);
        static constexpr uint64_t loop2 = loop_iterations(rest1, 2);
        static constexpr uint64_t rest2 = rest_cycles(rest1, 2);
        static constexpr uint64_t loop3 = loop_iterations(rest2, 3);

        static inline __attribute__((always_inline)) void place() {
            asm volatile(
            "    ldi  r18, %[loop0]"    "\n"
            "    ldi  r19, %[loop1]"    "\n"
            "    ldi  r20, %[loop2]"    "\n"
            "    ldi  r21, %[loop3]"    "\n"
            "1:  dec  r21"                "\n"
            "    brne 1b"                "\n"
            "    dec  r20"                "\n"
            "    brne 1b"                "\n"
            "    dec  r19"                "\n"
            "    brne 1b"                "\n"
            "    dec  r18"                "\n"
            "    brne 1b"                "\n"
            :
            :[loop0] "i"(loop0),[loop1] "i"(loop1),[loop2] "i"(loop2),[loop3] "i"(loop3)
            : "r18", "r19", "r20", "r21"
            );
        }
    };

    template<uint64_t Count>
    struct nop {
        static constexpr uint64_t get_depth(uint64_t cycles, uint64_t length, uint64_t d) {
            return cycles > length * 256 + d + 8 ? get_depth(cycles, length * 256 + 2, d + 1) : d;
        }

        static constexpr uint64_t depth = get_depth(Count, 3, 1);

        using nop_depth_type = nop_depth<Count, depth>;

        static inline __attribute__((always_inline)) void place() {
            nop_depth_type::place();
        }
    };
}}

#endif

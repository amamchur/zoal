#ifndef ZOAL_ARCH_ATMEL_AVR_NOOP_CYCLES_HPP
#define ZOAL_ARCH_ATMEL_AVR_NOOP_CYCLES_HPP

#include <stdint.h>
//
// Based on AVR delay Loop Calculator approach
// See http://www.bretmulvey.com/avrdelay.html
//
namespace zoal { namespace utils {
	template<uint64_t count, uint64_t depth> struct nop_depth {
		static_assert(true, "NopDepth should not be used directly");

		static inline void place() {
		}
	};

	template<> struct nop_depth<0, 1> {
		static inline void place() {
		}
	};

	template<> struct nop_depth<1, 1> {
		static inline void place() {
			asm volatile("nop"			"\n\t");
		}
	};

	template<> struct nop_depth<2, 1> {
		static inline void place() {
			asm volatile("rjmp .+0"		"\n\t");
		}
	};

	template<> struct nop_depth<3, 1> {
		static inline void place() {
			asm volatile(
				"lpm"			"\n\t"
			);
		}
	};

	template<> struct nop_depth<4, 1> {
		static inline void place() {
			asm volatile(
				"rjmp .+0"		"\n\t"
				"rjmp .+0"		"\n\t"
			);
		}
	};

	template<> struct nop_depth<5, 1> {
		static inline void place() {
			asm volatile(
				"rjmp .+0"		"\n\t"
				"lpm"			"\n\t"
			);
		}
	};

	template<> struct nop_depth<6, 1> {
		static inline void place() {
			asm volatile(
				"lpm"			"\n\t"
				"lpm"			"\n\t"
			);
		}
	};

	template<> struct nop_depth<7, 1> {
		static inline void place() {
			asm volatile(
				"lpm"			"\n\t"
				"lpm"			"\n\t"
				"nop"			"\n\t"
			);
		}
	};

	template<> struct nop_depth<8, 1> {
		static inline void place() {
			asm volatile(
				"lpm"			"\n\t"
				"lpm"			"\n\t"
				"rjmp .+0"		"\n\t"
			);
		}
	};

	template<> struct nop_depth<9, 1> {
		static inline void place() {
			asm volatile(
				"lpm"			"\n\t"
				"lpm"			"\n\t"
				"lpm"			"\n\t"
			);
		}
	};

	template <uint64_t depth>
	struct NopCalculator {
		static constexpr uint64_t Min(uint64_t a, uint64_t b) {
			return a < b ? a : b;
		}

		static constexpr uint64_t Max(uint64_t a, uint64_t b) {
			return a > b ? a : b;
		}

		static constexpr uint64_t Limit(uint64_t value) {
			return Max(1, Min(256, value));
		}

		static constexpr uint64_t CheckOverflow(uint64_t value) {
			return value == 256 ? 0 : value;
		}

		static constexpr uint64_t CyclesInLoop(uint64_t d) {
			return d <= 1 ? 3 : (CyclesInLoop(d - 1) * 256 + 2);
		}

		static constexpr uint64_t BranchCycles(uint64_t loop) {
			return 3 * (depth - loop);
		}

		static constexpr uint64_t LoopIterations(uint64_t cycles, uint64_t loop) {
			return CheckOverflow(Limit((cycles - BranchCycles(loop)) / CyclesInLoop(depth - loop) + 1));
		}

		static constexpr uint64_t RestCycles(uint64_t cycles, uint64_t loop) {
			return cycles - (LoopIterations(cycles, loop) - 1) * CyclesInLoop(depth - loop) - 3;
		}
	};

	template<uint64_t count> struct nop_depth<count, 1> : public NopCalculator<1> {
		static constexpr uint64_t Loop0 = LoopIterations(count, 0);
		static constexpr uint64_t Rest0 = RestCycles(count, 0);
		
		static inline void place() {
			asm volatile(
				"		ldi  r18, %[loop0]"	"\n\t"
				"1:		dec  r18"			"\n\t"
				"		brne 1b"			"\n\t"
				:
				: [loop0] "i" (Loop0)
				: "r18"
			);
		}
	};

	template<uint64_t count> struct nop_depth<count, 2> : public NopCalculator<2> {
		static constexpr uint64_t Loop0 = LoopIterations(count, 0);
		static constexpr uint64_t Rest0 = RestCycles(count, 0);
		static constexpr uint64_t Loop1 = LoopIterations(Rest0, 1);
		static constexpr uint64_t Rest1 = RestCycles(Rest0, 1);

		static inline void place() {
			asm volatile(
				"    ldi  r18, %[loop0]"	"\n"
				"    ldi  r19, %[loop1]"	"\n"
				"1:  dec  r19"				"\n"
				"    brne 1b"				"\n"
				"    dec  r18"				"\n"
				"    brne 1b"				"\n"
				:
				: [loop0] "i" (Loop0), [loop1] "i" (Loop1)
				: "r18", "r19"
			);
		}
	};

	template<uint64_t count> struct nop_depth<count, 3> : public NopCalculator<3> {
		static constexpr uint64_t Loop0 = LoopIterations(count, 0);
		static constexpr uint64_t Rest0 = RestCycles(count, 0);
		static constexpr uint64_t Loop1 = LoopIterations(Rest0, 1);
		static constexpr uint64_t Rest1 = RestCycles(Rest0, 1);
		static constexpr uint64_t Loop2 = LoopIterations(Rest1, 2);
		static constexpr uint64_t Rest2 = RestCycles(Rest1, 2);

		static inline void place() {
			asm volatile(
				"    ldi  r18, %[loop0]"	"\n"
				"    ldi  r19, %[loop1]"	"\n"
				"    ldi  r20, %[loop2]"	"\n"
				"1:  dec  r20"				"\n"
				"    brne 1b"				"\n"
				"    dec  r19"				"\n"
				"    brne 1b"				"\n"
				"    dec  r18"				"\n"
				"    brne 1b"				"\n"
				:
				: [loop0] "i" (Loop0), [loop1] "i" (Loop1), [loop2] "i" (Loop2)
				: "r18", "r19", "r20"
			);
		}
	};

	template<uint64_t count> struct nop_depth<count, 4> : public NopCalculator<4> {
		static constexpr uint64_t Loop0 = LoopIterations(count, 0);
		static constexpr uint64_t Rest0 = RestCycles(count, 0);
		static constexpr uint64_t Loop1 = LoopIterations(Rest0, 1);
		static constexpr uint64_t Rest1 = RestCycles(Rest0, 1);
		static constexpr uint64_t Loop2 = LoopIterations(Rest1, 2);
		static constexpr uint64_t Rest2 = RestCycles(Rest1, 2);
		static constexpr uint64_t Loop3 = LoopIterations(Rest2, 3);
		static constexpr uint64_t Rest3 = RestCycles(Rest2, 3);
		
		static inline void place() {
			asm volatile(
				"    ldi  r18, %[loop0]"	"\n"
				"    ldi  r19, %[loop1]"	"\n"
				"    ldi  r20, %[loop2]"	"\n"
				"    ldi  r21, %[loop3]"	"\n"
				"1:  dec  r21"				"\n"
				"    brne 1b"				"\n"
				"    dec  r20"				"\n"
				"    brne 1b"				"\n"
				"    dec  r19"				"\n"
				"    brne 1b"				"\n"
				"    dec  r18"				"\n"
				"    brne 1b"				"\n"
				:
				: [loop0] "i" (Loop0), [loop1] "i" (Loop1), [loop2] "i" (Loop2), [loop3] "i" (Loop3)
				: "r18", "r19", "r20", "r21"
			);
		}
	};

	template<uint64_t count> struct nop {
		static constexpr uint64_t GetDepth(uint64_t cycles, uint64_t length, uint64_t d) {
  			return cycles > length * 256 + d + 8 ? GetDepth(cycles, length * 256 + 2, d + 1) : d;
		}

		static constexpr uint64_t Depth = GetDepth(count, 3, 1);

		typedef nop_depth<count, Depth> NCD;

		static inline void place() {
			NCD::place();
		}
	};
}}

#endif

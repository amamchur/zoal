#ifndef ZOAL_ARCH_ATMEL_AVR_INTERRUPTS_HPP
#define ZOAL_ARCH_ATMEL_AVR_INTERRUPTS_HPP

#include <stdint.h>
#include <avr/interrupt.h>

namespace zoal { namespace utils {
	class interrupts {
	public:
		explicit inline interrupts(bool enable) {
			sREG = SREG;
			if (enable) {
				on();
			} else {
				off();
			}
		}

		inline ~interrupts() {
		 	SREG = sREG;
		}

		static inline void off() {
			asm volatile("cli"			"\n\t");
		}

		static inline void on() {
			asm volatile("sei"			"\n\t");
		}

	private:
		volatile uint8_t sREG;
	};
}}

#endif

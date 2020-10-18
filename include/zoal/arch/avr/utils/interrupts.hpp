#ifndef ZOAL_ARCH_ATMEL_AVR_INTERRUPTS_HPP
#define ZOAL_ARCH_ATMEL_AVR_INTERRUPTS_HPP

#include <stdint.h>
#include <avr/interrupt.h>

namespace zoal { namespace utils {
	class interrupts {
	public:
		explicit interrupts(bool enable) {
			sREG = SREG;
			if (enable) {
				on();
			} else {
				off();
			}
		}

        ZOAL_INLINE_IO ~interrupts() {
		 	SREG = sREG;
		}

        ZOAL_INLINE_IO static void off() {
			asm volatile("cli"			"\n\t");
		}

		ZOAL_INLINE_IO static void on() {
			asm volatile("sei"			"\n\t");
		}

	private:
		volatile uint8_t sREG;
	};
}}

#endif

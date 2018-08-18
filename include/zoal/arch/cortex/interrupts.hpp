#ifndef ZOAL_ARCH_CORTEX_INTERRUPTS_HPP
#define ZOAL_ARCH_CORTEX_INTERRUPTS_HPP

#include <stdint.h>

namespace zoal { namespace utils {
	class interrupts {
	public:
		inline interrupts(bool enable) {
			primask = getPRIMASK();
			if (enable) {
				on();
			} else {
				off();
			}
		}

		inline ~interrupts() {
		 	setPRIMASK(primask);
		}

		static inline void off() {
			asm volatile ("cpsid i" : : : "memory");
		}

		static inline void on() {
			asm volatile ("cpsie i" : : : "memory");
		}
	private:
		static inline uint32_t getPRIMASK() {
  			uint32_t result;	
  			asm volatile ("MRS %0, primask" : "=r" (result) );
  			return result;
		}

		static inline void setPRIMASK(uint32_t priMask) {
			asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
		}

		volatile uint32_t primask;
	};
}}

#endif

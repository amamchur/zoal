#pragma once

#ifndef ZOAL_ARCH_STM32X_INTERRUPT_CONTROL_HPP
#define ZOAL_ARCH_STM32X_INTERRUPT_CONTROL_HPP

#include <stdint.h>

namespace zoal { namespace utils {
	template <class NVIController, uint8_t Index, uint32_t Mask>
	class interrupt_control {
	public:
		static inline void enable_irq() {
			NVIController::instance()->ISER[Index] |= Mask;
		}

		static inline void disable_irq() {
			NVIController::instance()->ICER[Index] &= ~Mask;
		}
	};
}}

#endif

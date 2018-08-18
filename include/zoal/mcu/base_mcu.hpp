#ifndef ZOAL_MCU_BASE_HPP
#define ZOAL_MCU_BASE_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace mcu {
	template<uint32_t freq, uint8_t ws>
	class base_mcu {
	public:
		static constexpr uint32_t frequency = freq;
		static constexpr uint32_t ps_per_clock  = (1000000000 / (freq / 1000));
		static constexpr uint32_t word_size = ws;
	};
}}

#endif

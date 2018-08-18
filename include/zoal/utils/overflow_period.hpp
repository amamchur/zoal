#ifndef ZOAL_UTILS_OVERFLOW_PERIOD_HPP
#define ZOAL_UTILS_OVERFLOW_PERIOD_HPP

#include <stdint.h>

namespace zoal { namespace utils {
	template <uint32_t Freq, uint32_t Prescaler, uint16_t TimerOverflow>
	class overflow_period {
	public:
		static constexpr uint32_t overflow_frequency = Freq / Prescaler / TimerOverflow;
		static constexpr uint32_t picoseconds = 1000000000 / overflow_frequency;
		static constexpr uint32_t microseconds= 1000000 / overflow_frequency;
		static constexpr uint32_t milliseconds = 1000 / overflow_frequency;
	};
}}

#endif
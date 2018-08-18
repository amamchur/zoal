#ifndef ZOAL_PERIPH_TIMER_MODE_HPP
#define ZOAL_PERIPH_TIMER_MODE_HPP

namespace zoal { namespace periph {
	enum class timer_mode {
		normal,

		fast_pwm_8bit,
		fast_pwm_9bit,
		fast_pwm_10bit,
		fast_pwm_16bit,

		phase_correct_8bit,
		phase_correct_9bit,
		phase_correct_10bit,
		phase_correct_16bit
	};
}}

#endif

#ifndef ZOAL_GPIO_ATMEL_AVR_MODEL_TYPE_HPP
#define ZOAL_GPIO_ATMEL_AVR_MODEL_TYPE_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace avr {
	enum class mcu_type {
		atmega,
		attiny13,
		attiny2313
	};
}}}

#endif

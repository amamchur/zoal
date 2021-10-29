#ifndef ROBOTO_REGULAR_16
#define ROBOTO_REGULAR_16
#include <zoal/text/types.hpp>

#ifdef __AVR_ARCH__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

extern const zoal::text::font roboto_regular_16;
#endif

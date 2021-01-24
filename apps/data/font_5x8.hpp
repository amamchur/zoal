#ifndef ZOAL_FONT_5X8_HPP
#define ZOAL_FONT_5X8_HPP

#ifdef __AVR_ARCH__
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

extern const unsigned char font_5x8[] PROGMEM;

#endif


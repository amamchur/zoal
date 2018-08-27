#ifndef ZOAL_ARCH_ATMEL_AVR_PROG_MEM_READER_HPP
#define ZOAL_ARCH_ATMEL_AVR_PROG_MEM_READER_HPP

#include <stdint.h>
#include <avr/pgmspace.h>

namespace zoal { namespace utils {
	class program_mem_reader {
	public:
		template <class T>
		static void copy(T *dest, const T *src) {
			auto *d = (uint8_t *)dest;
			auto *s = (uint8_t *)src;
			for (auto i = 0; i < sizeof(T); i++) {
				*d++ = pgm_read_byte(s++);
			}
		}
	};
}}

#endif

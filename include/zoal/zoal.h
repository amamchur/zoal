#ifndef ZOAL_H
#define ZOAL_H

#include <stddef.h>

namespace zoal {
	namespace gpio {}
	namespace io {}
	namespace ic {}
	namespace mcu {}
	namespace data {}
	namespace utils {}
}

void *operator new(size_t size) __attribute__((weak));
void *operator new[](size_t size) __attribute__((weak));

void operator delete(void * ptr) noexcept __attribute__((weak));
void operator delete[](void * ptr) noexcept __attribute__((weak));

#endif

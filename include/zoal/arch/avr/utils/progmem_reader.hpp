#ifndef ZOAL_PROGMEM_READER_HPP
#define ZOAL_PROGMEM_READER_HPP

#include <avr/pgmspace.h>

namespace zoal { namespace utils {
    class progmem_reader {
    public:
        template<class T>
        static T read_mem(const void *ptr) {
            T t;
            memcpy_P(&t, ptr, sizeof(t));
            return t;
        }
    };
}}

#endif //ZOAL_PROGMEM_READER_HPP

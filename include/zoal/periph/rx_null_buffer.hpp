#ifndef ZOAL_RX_NULL_BUFFER_HPP
#define ZOAL_RX_NULL_BUFFER_HPP

#include <stdint.h>

namespace zoal { namespace periph {
    class rx_null_buffer {
    public:
        static inline bool empty() {
            return true;
        }

        static inline void put(uint8_t data) {}

        static inline void flush() {}
    };
}}

#endif

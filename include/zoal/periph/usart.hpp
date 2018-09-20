#ifndef ZOAL_PERIPH_USART_CONFIG_HPP
#define ZOAL_PERIPH_USART_CONFIG_HPP

#include <stdint.h>

namespace zoal { namespace periph {
    enum class usart_parity {
        none,
        even,
        odd
    };

    enum class usart_stop_bits {
        stop_bits_1,
        stop_bits_2
    };
}}

#endif

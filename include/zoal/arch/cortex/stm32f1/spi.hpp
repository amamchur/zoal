#ifndef ZOAL_ARCH_STM32F1_SPI_HPP
#define ZOAL_ARCH_STM32F1_SPI_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class Clock>
    class spi : public Clock {
    public:
        using self_type = spi<Address, Clock>;
        using register_type = uint32_t;
    };
}}}

#endif

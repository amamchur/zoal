#ifndef ZOAL_ARCH_STM32F1_AFIO_HPP
#define ZOAL_ARCH_STM32F1_AFIO_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class Clock>
    class afio : public Clock {
    public:
        using self_type = afio<Address, Clock>;

        static constexpr uintptr_t address = Address;

        static constexpr uintptr_t AFIOx_EVCR = 0x00;
        static constexpr uintptr_t AFIOx_MAPR = 0x04;
        static constexpr uintptr_t AFIOx_EXTICR1 = 0x08;
        static constexpr uintptr_t AFIOx_EXTICR2 = 0x0C;
        static constexpr uintptr_t AFIOx_EXTICR3 = 0x10;
        static constexpr uintptr_t AFIOx_EXTICR4 = 0x14;
        static constexpr uintptr_t AFIOx_MAPR2 = 0x1C;
    };
}}}

#endif

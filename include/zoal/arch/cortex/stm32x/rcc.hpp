#ifndef ZOAL_GPIO_STM32X_RCC_HPP
#define ZOAL_GPIO_STM32X_RCC_HPP

#include "../../../arch/bus.hpp"
#include "bus_clock.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address = 0x40021000>
    class rcc {
    public:
        using self_type = rcc<Address>;

        static constexpr uintptr_t address = Address;

        static constexpr uintptr_t RCCx_CR = 0x000;
        static constexpr uintptr_t RCCx_CFGR = 0x004;
        static constexpr uintptr_t RCCx_CIR = 0x008;
        static constexpr uintptr_t RCCx_APB2RSTR = 0x00C;
        static constexpr uintptr_t RCCx_APB1RSTR = 0x010;
        static constexpr uintptr_t RCCx_AHBENR = 0x014;
        static constexpr uintptr_t RCCx_APB2ENR = 0x018;
        static constexpr uintptr_t RCCx_APB1ENR = 0x01C;
        static constexpr uintptr_t RCCx_BDCR = 0x020;
        static constexpr uintptr_t RCCx_CSR = 0x024;
        static constexpr uintptr_t RCCx_AHBRSTR = 0x028;
        static constexpr uintptr_t RCCx_CFGR2 = 0x02C;
    };
}}}

#endif

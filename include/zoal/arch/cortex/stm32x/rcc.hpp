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

        using RCCx_CR = zoal::mem::reg<Address + 0x000, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CFGR = zoal::mem::reg<Address + 0x004, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CIR = zoal::mem::reg<Address + 0x008, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB2RSTR = zoal::mem::reg<Address + 0x00C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB1RSTR = zoal::mem::reg<Address + 0x010, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHBENR = zoal::mem::reg<Address + 0x014, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB2ENR = zoal::mem::reg<Address + 0x018, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB1ENR = zoal::mem::reg<Address + 0x01C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_BDCR = zoal::mem::reg<Address + 0x020, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CSR = zoal::mem::reg<Address + 0x024, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHBRSTR = zoal::mem::reg<Address + 0x028, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CFGR2 = zoal::mem::reg<Address + 0x02C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
    };
}}}

#endif

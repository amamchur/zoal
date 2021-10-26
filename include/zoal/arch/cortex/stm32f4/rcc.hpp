#ifndef ZOAL_GPIO_STM32F4_RCC_HPP
#define ZOAL_GPIO_STM32F4_RCC_HPP

#include "../../../arch/bus.hpp"
#include "../stm32x/bus_clock.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f4 {
    template<uintptr_t Address>
    class rcc {
    public:
        using self_type = rcc<Address>;

        static constexpr uintptr_t address = Address;

        using RCCx_CR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_PLLCFGR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CFGR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CIR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB1RSTR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB2RSTR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB3RSTR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB1RSTR = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB2RSTR = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB1ENR = zoal::mem::reg<Address + 0x30, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB2ENR = zoal::mem::reg<Address + 0x34, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB3ENR = zoal::mem::reg<Address + 0x38, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB1ENR = zoal::mem::reg<Address + 0x40, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB2ENR = zoal::mem::reg<Address + 0x44, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB1LPENR = zoal::mem::reg<Address + 0x50, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB2LPENR = zoal::mem::reg<Address + 0x54, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_AHB3LPENR = zoal::mem::reg<Address + 0x58, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB1LPENR = zoal::mem::reg<Address + 0x60, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_APB2LPENR = zoal::mem::reg<Address + 0x64, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_BDCR = zoal::mem::reg<Address + 0x70, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_CSR = zoal::mem::reg<Address + 0x74, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_SSCGR = zoal::mem::reg<Address + 0x80, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_PLLI2SCFGR = zoal::mem::reg<Address + 0x84, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using RCCx_DCKCFGR = zoal::mem::reg<Address + 0x8C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
    };
}}}

#endif

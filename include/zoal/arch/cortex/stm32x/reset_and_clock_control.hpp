#ifndef ZOAL_GPIO_STM32X_RESET_AND_CLOCK_CONTROLL_HPP
#define ZOAL_GPIO_STM32X_RESET_AND_CLOCK_CONTROLL_HPP

#include "../../../arch/bus.hpp"
#include "zoal/mem/segment.hpp"
#include "bus_clock_control.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address = 0x40021000>
    class reset_and_clock_control {
    public:
        using self_type = reset_and_clock_control<Address>;

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

        reset_and_clock_control() = delete;

    private:
        template<class RCController, zoal::arch::bus Bus, uint32_t SetMask, uint32_t ClearMask = ~SetMask>
        friend class ::zoal::arch::stm32x::bus_clock_control;

        static zoal::mem::segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address>
    zoal::mem::segment<uint32_t, Address> reset_and_clock_control<Address>::mem;
}}}

#endif

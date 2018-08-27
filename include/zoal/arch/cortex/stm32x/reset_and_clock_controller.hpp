#pragma once

#ifndef ZOAL_GPIO_STM32X_RESET_AND_CLOCK_CONTROLLER_HPP
#define ZOAL_GPIO_STM32X_RESET_AND_CLOCK_CONTROLLER_HPP

#include <stdint.h>
#include "../../../utils/memory_segment.hpp"
#include "peripheral_clock.hpp"

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address>
    class reset_and_clock_controller {
    public:
        using self_type = reset_and_clock_controller<Address>;

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

        volatile uint32_t CR;             /**< Clock control register */
        volatile uint32_t CFGR;           /**< Clock configuration register */
        volatile uint32_t CIR;            /**< Clock interrupt register */
        volatile uint32_t APB2RSTR;       /**< APB2 peripheral reset register */
        volatile uint32_t APB1RSTR;       /**< APB1 peripheral reset register */
        volatile uint32_t AHBENR;         /**< AHB peripheral clock enable register */
        volatile uint32_t APB2ENR;        /**< APB2 peripheral clock enable register */
        volatile uint32_t APB1ENR;        /**< APB1 peripheral clock enable register */
        volatile uint32_t BDCR;           /**< Backup domain control register */
        volatile uint32_t CSR;            /**< Control/status register */
        volatile uint32_t AHBRSTR;
        volatile uint32_t CFGR2;
        volatile uint32_t CFGR3;

        reset_and_clock_controller() = delete;

        static constexpr inline self_type *instance() {
            return (self_type *) Address;
        }

    private:
        template<class RCController, rcc_register Register, uint32_t SetMask, uint32_t ClearMask = ~SetMask>
        friend class ::zoal::arch::stm32x::clock_control;

        static zoal::utils::memory_segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address>
    zoal::utils::memory_segment<uint32_t, Address> reset_and_clock_controller<Address>::mem;
}}}

#endif

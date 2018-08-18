#pragma once

#ifndef ZOAL_GPIO_STM32X_RESET_AND_CLOCK_CONTROLLER_HPP
#define ZOAL_GPIO_STM32X_RESET_AND_CLOCK_CONTROLLER_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    template<uintptr_t Address>
    class reset_and_clock_controller {
    public:
        using self_type = reset_and_clock_controller<Address>;

        static constexpr uintptr_t Base = Address;

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
    };
}}}

#endif

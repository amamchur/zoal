#pragma once

#ifndef ZOAL_ARCH_STM32X_PERIPHERALCLOCK_HPP
#define ZOAL_ARCH_STM32X_PERIPHERALCLOCK_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {
    enum class rcc_register {
        AHBENR,
        APB1ENR,
        APB2ENR,
        CFGR2
    };

    template<class RCController, rcc_register Register, uint32_t SetMask, uint32_t ClearMask = ~SetMask>
    class clock_control {
    public:
        using rcc = RCController;
        static constexpr uint32_t set_mask = SetMask;
        static constexpr uint32_t clear_mask = ClearMask;

        static void enable() {
            switch (Register) {
                case rcc_register::AHBENR:
                    rcc::mem[rcc::RCCx_AHBENR] |= SetMask;
                    break;
                case rcc_register::APB1ENR:
                    rcc::mem[rcc::RCCx_APB1ENR] |= SetMask;
                    break;
                case rcc_register::APB2ENR:
                    rcc::mem[rcc::RCCx_APB2ENR] |= SetMask;
                    break;
                case rcc_register::CFGR2:
                    rcc::mem[rcc::RCCx_CFGR2] |= SetMask;
                    break;
            }
        }

        static void disable() {
            switch (Register) {
                case rcc_register::AHBENR:
                    rcc::mem[rcc::RCCx_AHBENR] &= ClearMask;
                    break;
                case rcc_register::APB1ENR:
                    rcc::mem[rcc::RCCx_APB1ENR] &= ClearMask;
                    break;
                case rcc_register::APB2ENR:
                    rcc::mem[rcc::RCCx_APB2ENR] &= ClearMask;
                    break;
                case rcc_register::CFGR2:
                    rcc::mem[rcc::RCCx_CFGR2] &= ClearMask;
                    break;
            }
        }
    };

    template<class First, class ... Rest>
    class clock_control_set {
    public:
        using next = clock_control_set<Rest...>;

        static inline void enable() {
            First::enable();
            next::enable();
        }

        static inline void disable() {
            First::disable();
            next::disable();
        }
    };

    template<class First>
    class clock_control_set<First> {
    public:
        static inline void enable() {
            First::enable();
        }

        static inline void disable() {
            First::disable();
        }
    };

    template<class RCController, uint32_t MaskAHBENR, uint32_t MaskAPB1ENR, uint32_t MaskAPB2ENR>
    class peripheral_clock {
    };

    template<class RCController, uint32_t MaskAHBENR>
    class peripheral_clock<RCController, MaskAHBENR, 0, 0> {
    public:
        using rcc = RCController;

        static inline void enable() {
            rcc::instance()->AHBENR |= MaskAHBENR;
        }

        static inline void disable() {
            rcc::instance()->AHBENR &= ~MaskAHBENR;
        }
    };

    template<class RCController, uint32_t MaskAPB1ENR>
    class peripheral_clock<RCController, 0, MaskAPB1ENR, 0> {
    public:
        using rcc = RCController;

        static inline void enable() {
            rcc::instance()->APB1ENR |= MaskAPB1ENR;
        }

        static inline void disable() {
            rcc::instance()->APB1ENR &= ~MaskAPB1ENR;
        }
    };

    template<class RCController, uint32_t MaskAPB2ENR>
    class peripheral_clock<RCController, 0, 0, MaskAPB2ENR> {
    public:
        using rcc = RCController;

        static inline void enable() {
            rcc::instance()->APB2ENR |= MaskAPB2ENR;
        }

        static inline void disable() {
            rcc::instance()->APB2ENR &= ~MaskAPB2ENR;
        }
    };
}}}

#endif

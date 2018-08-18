#pragma once

#ifndef ZOAL_ARCH_STM32X_PERIPHERALCLOCK_HPP
#define ZOAL_ARCH_STM32X_PERIPHERALCLOCK_HPP

#include <stdint.h>

namespace zoal { namespace stm32x {
    enum class RCCReg {
        AHBENR,
        APB1ENR,
        APB2ENR,
        CFGR2
    };

    template<class RCCtrl, RCCReg reg, uint32_t set, uint32_t clear = ~set>
    class peripheral_option {
    public:
        static constexpr uint32_t SetMask = set;
        static constexpr uint32_t ClearMask = clear;

        static void enable() {
            disable();

            auto rcc = RCCtrl::instance();
            switch (reg) {
                case RCCReg::AHBENR:
                    rcc->AHBENR |= set;
                    break;
                case RCCReg::APB1ENR:
                    rcc->APB1ENR |= set;
                    break;
                case RCCReg::APB2ENR:
                    rcc->APB2ENR |= set;
                    break;
                case RCCReg::CFGR2:
                    rcc->CFGR2 |= set;
                    break;
            }
        }

        static void disable() {
            auto rcc = RCCtrl::instance();
            switch (reg) {
                case RCCReg::AHBENR:
                    rcc->AHBENR &= clear;
                    break;
                case RCCReg::APB1ENR:
                    rcc->APB1ENR &= clear;
                    break;
                case RCCReg::APB2ENR:
                    rcc->APB2ENR &= clear;
                    break;
                case RCCReg::CFGR2:
                    rcc->CFGR2 &= clear;
                    break;
            }
        }
    };

    template<class First, class ... Rest>
    class peripheral_option_set {
    public:
        using Next = peripheral_option_set<Rest...>;

        static inline void enable() {
            First::enable();
            Next::enable();
        }
    };

    template<class First>
    class peripheral_option_set<First> {
    public:
        static inline void enable() {
            First::enable();
        }
    };

    template<class RCCtrl, uint32_t AHBENR, uint32_t APB1ENR, uint32_t APB2ENR>
    class peripheral_clock {
    };

    template<class RCCtrl, uint32_t AHBENR>
    class peripheral_clock<RCCtrl, AHBENR, 0, 0> {
    public:
        using RCController = RCCtrl;

        static inline void enable() {
            RCController::instance()->AHBENR |= AHBENR;
        }

        static inline void disable() {
            RCController::instance()->AHBENR &= ~AHBENR;
        }
    };

    template<class RCCtrl, uint32_t APB1ENR>
    class peripheral_clock<RCCtrl, 0, APB1ENR, 0> {
    public:
        using RCController = RCCtrl;

        static inline void enable() {
            RCController::instance()->APB1ENR |= APB1ENR;
        }

        static inline void disable() {
            RCController::instance()->APB1ENR &= ~APB1ENR;
        }
    };

    template<class RCCtrl, uint32_t APB2ENR>
    class peripheral_clock<RCCtrl, 0, 0, APB2ENR> {
    public:
        using RCController = RCCtrl;

        static inline void enable() {
            RCController::instance()->APB2ENR |= APB2ENR;
        }

        static inline void disable() {
            RCController::instance()->APB2ENR &= ~APB2ENR;
        }
    };
}}

#endif

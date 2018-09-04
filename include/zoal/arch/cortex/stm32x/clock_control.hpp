#pragma once

#ifndef ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP

#include <stdint.h>

#include "../bus.hpp"

namespace zoal { namespace arch { namespace stm32x {
    template<class ResetAndClockControl, zoal::arch::cortex::bus Bus, uint32_t SetMask, uint32_t ClearMask = ~SetMask>
    class clock_control {
    public:
        using rcc = ResetAndClockControl;
        static constexpr uint32_t set_mask = SetMask;
        static constexpr uint32_t clear_mask = ClearMask;

        static inline void power_on() {
            using namespace zoal::arch::cortex;
            switch (Bus) {
            case bus::AHB:
                rcc::mem[rcc::RCCx_AHBENR] |= SetMask;
                break;
            case bus::APB1:
                rcc::mem[rcc::RCCx_APB1ENR] |= SetMask;
                break;
            case bus::APB2:
                rcc::mem[rcc::RCCx_APB2ENR] |= SetMask;
                break;
            }
        }

        static inline void power_off() {
            using namespace zoal::arch::cortex;
            switch (Bus) {
            case bus::AHB:
                rcc::mem[rcc::RCCx_AHBENR] &= ClearMask;
                break;
            case bus::APB1:
                rcc::mem[rcc::RCCx_APB1ENR] &= ClearMask;
                break;
            case bus::APB2:
                rcc::mem[rcc::RCCx_APB2ENR] &= ClearMask;
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
}}}

#endif

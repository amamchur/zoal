#pragma once

#ifndef ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP

#include <stdint.h>

#include "zoal/arch/cortex/bus_type.hpp"

namespace zoal { namespace arch { namespace stm32x {
    template<class ResetAndClockControl, zoal::arch::cortex::bus_type Bus, uint32_t SetMask, uint32_t ClearMask = ~SetMask>
    class bus_clock_control {
    public:
        using rcc = ResetAndClockControl;
        static constexpr zoal::arch::cortex::bus_type bus = Bus;
        static constexpr uint32_t set_mask = SetMask;
        static constexpr uint32_t clear_mask = ClearMask;

        static inline void power_on() {
            using bt = zoal::arch::cortex::bus_type;
            switch (Bus) {
            case bt::AHB:
                rcc::mem[rcc::RCCx_AHBENR] |= SetMask;
                break;
            case bt::APB1:
                rcc::mem[rcc::RCCx_APB1ENR] |= SetMask;
                break;
            case bt::APB2:
                rcc::mem[rcc::RCCx_APB2ENR] |= SetMask;
                break;
            }
        }

        static inline void power_off() {
            using namespace zoal::arch::cortex;
            switch (Bus) {
            case bus_type::AHB:
                rcc::mem[rcc::RCCx_AHBENR] &= ClearMask;
                break;
            case bus_type::APB1:
                rcc::mem[rcc::RCCx_APB1ENR] &= ClearMask;
                break;
            case bus_type::APB2:
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

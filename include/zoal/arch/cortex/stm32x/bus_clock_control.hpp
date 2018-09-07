#pragma once

#ifndef ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP

#include <stdint.h>

#include "../../../arch/bus.hpp"

namespace zoal { namespace arch { namespace stm32x {
    template<class ResetAndClockControl, zoal::arch::bus Bus, uint32_t SetMask, uint32_t ClearMask = ~SetMask>
    class bus_clock_control {
    public:
        using rcc = ResetAndClockControl;
        static constexpr zoal::arch::bus bus = Bus;

        static inline void power_on() {
            using bt = zoal::arch::bus;
            switch (Bus) {
            case bt::cortex_ahb:
                rcc::mem[rcc::RCCx_AHBENR] |= SetMask;
                break;
            case bt::cortex_apb1:
                rcc::mem[rcc::RCCx_APB1ENR] |= SetMask;
                break;
            case bt::cortex_apb2:
                rcc::mem[rcc::RCCx_APB2ENR] |= SetMask;
                break;
            default:
                break;
            }
        }

        static inline void power_off() {
            using bt = zoal::arch::bus;
            switch (Bus) {
            case bt::cortex_ahb:
                rcc::mem[rcc::RCCx_AHBENR] &= ClearMask;
                break;
            case bt::cortex_apb1:
                rcc::mem[rcc::RCCx_APB1ENR] &= ClearMask;
                break;
            case bt::cortex_apb2:
                rcc::mem[rcc::RCCx_APB2ENR] &= ClearMask;
                break;
            default:
                break;
            }
        }
    };
}}}

#endif

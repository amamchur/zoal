#ifndef ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP

#include "../../../arch/bus.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/modifier.hpp"
#include "../../../mem/segment.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {

    template<class Ctrl, zoal::arch::bus Bus, uint32_t Mask>
    class bus_clock_modifiers {
    public:
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_ahb, Mask> {
    public:
        using RCCx_AHBENR = zoal::mem::modifier<Ctrl::RCCx_AHBENR, uint32_t, Mask, Mask>;
        using RCCx_APB2ENR = zoal::mem::modifier<Ctrl::RCCx_APB2ENR, uint32_t, 0, 0>;
        using RCCx_APB1ENR = zoal::mem::modifier<Ctrl::RCCx_APB1ENR, uint32_t, 0, 0>;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_apb1, Mask> {
    public:
        using RCCx_AHBENR = zoal::mem::modifier<Ctrl::RCCx_AHBENR, uint32_t, 0, 0>;
        using RCCx_APB2ENR = zoal::mem::modifier<Ctrl::RCCx_APB2ENR, uint32_t, 0, 0>;
        using RCCx_APB1ENR = zoal::mem::modifier<Ctrl::RCCx_APB1ENR, uint32_t, Mask, Mask>;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_apb2, Mask> {
    public:
        using RCCx_AHBENR = zoal::mem::modifier<Ctrl::RCCx_AHBENR, uint32_t, 0, 0>;
        using RCCx_APB2ENR = zoal::mem::modifier<Ctrl::RCCx_APB2ENR, uint32_t, Mask, Mask>;
        using RCCx_APB1ENR = zoal::mem::modifier<Ctrl::RCCx_APB1ENR, uint32_t, 0, 0>;
    };

    template<class ResetAndClockControl, zoal::arch::bus Bus, uint32_t Mask>
    class bus_clock {
    public:
        using rcc = ResetAndClockControl;
        using clock_modifiers = bus_clock_modifiers<rcc, Bus, Mask>;
        using modifiers = zoal::ct::type_list<typename clock_modifiers::RCCx_AHBENR,
                                              typename clock_modifiers::RCCx_APB2ENR,
                                              typename clock_modifiers::RCCx_APB1ENR>;

        static constexpr zoal::arch::bus bus = Bus;

        static inline void power_on() {
            zoal::mem::apply_modifiers<rcc::address, modifiers>();
        }

        static inline void power_off() {
            zoal::mem::clear_modifiers<rcc::address, modifiers>();
        }
    };
}}}

#endif

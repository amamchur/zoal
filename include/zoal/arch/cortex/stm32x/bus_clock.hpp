#ifndef ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP

#include "../../../arch/bus.hpp"
#include "../../../gpio/api.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32x {

    template<class Ctrl, zoal::arch::bus Bus, uint32_t Mask>
    class bus_clock_modifiers {
    public:
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_ahb, Mask> {
    public:
        using RCCx_AHBENR = typename Ctrl::RCCx_AHBENR::template cas<Mask, Mask>;
        using RCCx_APB2ENR = typename Ctrl::RCCx_APB2ENR::template cas<0, 0>;
        using RCCx_APB1ENR = typename Ctrl::RCCx_APB1ENR::template cas<0, 0>;
        using list = zoal::ct::type_list<RCCx_AHBENR, RCCx_APB2ENR, RCCx_APB1ENR>;
        using all = typename zoal::gpio::api::optimize<list>::result;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_apb1, Mask> {
    public:
        using RCCx_AHBENR = typename Ctrl::RCCx_AHBENR::template cas<0, 0>;
        using RCCx_APB2ENR = typename Ctrl::RCCx_APB2ENR::template cas<0, 0>;
        using RCCx_APB1ENR = typename Ctrl::RCCx_APB1ENR::template cas<Mask, Mask>;

        using list = zoal::ct::type_list<RCCx_AHBENR, RCCx_APB2ENR, RCCx_APB1ENR>;
        using all = typename zoal::gpio::api::optimize<list>::result;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_apb2, Mask> {
    public:
        using RCCx_AHBENR = typename Ctrl::RCCx_AHBENR::template cas<0, 0>;
        using RCCx_APB2ENR = typename Ctrl::RCCx_APB2ENR::template cas<Mask, Mask>;
        using RCCx_APB1ENR = typename Ctrl::RCCx_APB1ENR::template cas<0, 0>;
        using list = zoal::ct::type_list<RCCx_AHBENR, RCCx_APB2ENR, RCCx_APB1ENR>;
        using all = typename zoal::gpio::api::optimize<list>::result;
    };

    template<class ResetAndClockControl, zoal::arch::bus Bus, uint32_t Mask>
    class bus_clock {
    public:
        using rcc = ResetAndClockControl;
        using clock_modifiers = bus_clock_modifiers<rcc, Bus, Mask>;
        static constexpr zoal::arch::bus bus = Bus;

        static inline void power_on() {
            zoal::mem::apply_cas_list<typename clock_modifiers::all>();
        }

        static inline void power_off() {
            zoal::mem::apply_cas_list<typename clock_modifiers::all>();
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP
#define ZOAL_ARCH_STM32X_CLOCK_CONTROL_HPP

#include "../../../arch/bus.hpp"
#include "../../../gpio/api.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32x {
    using zoal::ct::type_list;
    using zoal::gpio::api;

    template<class Ctrl, zoal::arch::bus Bus, uint32_t Mask>
    class bus_clock_modifiers {
    public:
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_ahb, Mask> {
    public:
        using clock_on_cas = type_list<typename Ctrl::RCCx_AHBENR::template cas<Mask, Mask>>;
        using clock_off_cas = type_list<typename Ctrl::RCCx_AHBENR::template cas<Mask, 0>>;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_ahb1, Mask> {
    public:
        using clock_on_cas = type_list<typename Ctrl::RCCx_AHB1ENR::template cas<Mask, Mask>>;
        using clock_off_cas = type_list<typename Ctrl::RCCx_AHB1ENR::template cas<Mask, 0>>;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_ahb2, Mask> {
    public:
        using clock_on_cas = type_list<typename Ctrl::RCCx_AHB2ENR::template cas<Mask, Mask>>;
        using clock_off_cas = type_list<typename Ctrl::RCCx_AHB2ENR::template cas<Mask, 0>>;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_apb1, Mask> {
    public:
        using clock_on_cas = type_list<typename Ctrl::RCCx_APB1ENR::template cas<Mask, Mask>>;
        using clock_off_cas = type_list<typename Ctrl::RCCx_APB1ENR::template cas<Mask, 0>>;
    };

    template<class Ctrl, uint32_t Mask>
    class bus_clock_modifiers<Ctrl, zoal::arch::bus::cortex_apb2, Mask> {
    public:
        using clock_on_cas = type_list<typename Ctrl::RCCx_APB2ENR::template cas<Mask, Mask>>;
        using clock_off_cas = type_list<typename Ctrl::RCCx_APB2ENR::template cas<Mask, 0>>;
    };

    template<class ResetAndClockControl, zoal::arch::bus Bus, uint32_t Mask>
    class bus_clock {
    public:
        using rcc = ResetAndClockControl;
        using power_cas = bus_clock_modifiers<rcc, Bus, Mask>;
        static constexpr zoal::arch::bus bus = Bus;

        using clock_on_cas = typename power_cas::clock_on_cas;
        using clock_off_cas =  typename power_cas::clock_off_cas;

        static inline void clock_on() {
            zoal::mem::apply_cas_list<typename power_cas::clock_on_cas>::apply();
        }

        static inline void clock_off() {
            zoal::mem::apply_cas_list<typename power_cas::clock_off_cas>::apply();
        }
    };
}}}

#endif

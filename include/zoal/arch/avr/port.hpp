#ifndef ZOAL_ARCH_AVR_PORT_HPP
#define ZOAL_ARCH_AVR_PORT_HPP

#include "../../ct/type_list.hpp"
#include "../../gpio/pin_mode.hpp"
#include "../../mem/accessor.hpp"
#include "../../mem/clear_and_set.hpp"
#include "../../mem/modifier.hpp"
#include "../../utils/defs.hpp"
#include "../bus.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<::zoal::gpio::pin_mode PinMode, uint8_t Mask>
    struct pin_mode_cfg {
        using DDRx = ::zoal::mem::clear_and_set<0, 0>;
        using PORTx = ::zoal::mem::clear_and_set<0, 0>;
    };

    template<uint8_t Mask>
    struct pin_mode_cfg<::zoal::gpio::pin_mode::input_floating, Mask> {
        using DDRx = ::zoal::mem::clear_and_set<Mask, 0>;
        using PORTx = ::zoal::mem::clear_and_set<Mask, 0>;
    };

    template<uint8_t Mask>
    struct pin_mode_cfg<::zoal::gpio::pin_mode::input_pull_up, Mask> {
        using DDRx = ::zoal::mem::clear_and_set<Mask, 0>;
        using PORTx = ::zoal::mem::clear_and_set<0, Mask>;
    };

    template<uint8_t Mask>
    struct pin_mode_cfg<::zoal::gpio::pin_mode::output_push_pull, Mask> {
        using DDRx = ::zoal::mem::clear_and_set<0, Mask>;
        using PORTx = ::zoal::mem::clear_and_set<0, 0>;
    };

    template<uintptr_t Address, uint8_t PinMask = 0xFF>
    class port {
    public:
        using self_type = port<Address, PinMask>;
        using register_type = uint8_t;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        template<intptr_t Offset, register_type Clear, register_type Set>
        using modifier = zoal::mem::modifier<Offset, register_type, Clear, Set>;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr auto address = Address;
        static constexpr uint8_t pin_mask = PinMask;
        static constexpr intptr_t PINx = 0x00;
        static constexpr intptr_t DDRx = 0x01;
        static constexpr intptr_t PORTx = 0x02;

        static void power_on() {}

        static void power_off() {
            mode<::zoal::gpio::pin_mode::input_floating, PinMask>();
        }

        static register_type read() {
            return *accessor<PINx>::p;
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void low() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            *accessor<PORTx>::p &= ~Mask;
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void high() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            *accessor<PORTx>::p |= Mask;
        }

        template<register_type Mask>
        static void toggle() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            *accessor<PORTx>::p ^= Mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        static void mode() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            using cfg = pin_mode_cfg<PinMode, Mask & pin_mask>;
            cfg::DDRx::apply(*accessor<DDRx>::p);
            cfg::PORTx::apply(*accessor<PORTx>::p);
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        struct mode_modifiers {
            using cfg = pin_mode_cfg<PinMode, Mask & pin_mask>;

            using modifiers = zoal::ct::type_list<modifier<DDRx, cfg::DDRx::clear_mask, cfg::DDRx::set_mask>,
                                                  modifier<PORTx, cfg::PORTx::clear_mask, cfg::PORTx::set_mask>>;

            mode_modifiers() {
                using fm = typename zoal::mem::filter_modifiers<modifiers>::result;
                zoal::mem::apply_modifiers<address, fm>();
            }
        };

        template<register_type Mask>
        struct low_modifiers {
            using modifiers = zoal::ct::type_list<modifier<DDRx, 0, 0>, modifier<PORTx, Mask, 0>>;

            low_modifiers() {
                using fm = typename zoal::mem::filter_modifiers<modifiers>::result;
                zoal::mem::apply_modifiers<address, fm>();
            }
        };

        template<register_type Mask>
        struct high_modifiers {
            using modifiers = zoal::ct::type_list<modifier<DDRx, 0, 0>, modifier<PORTx, 0, Mask>>;

            high_modifiers() {
                using fm = typename zoal::mem::filter_modifiers<modifiers>::result;
                zoal::mem::apply_modifiers<address, fm>();
            }
        };
    };
}}}

#endif

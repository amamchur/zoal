#ifndef ZOAL_ARCH_AVR_PORT_HPP
#define ZOAL_ARCH_AVR_PORT_HPP

#include "../../ct/type_list.hpp"
#include "../../gpio/pin_mode.hpp"
#include "../../mem/reg.hpp"
#include "../../utils/defs.hpp"
#include "../bus.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<class Port, ::zoal::gpio::pin_mode PinMode, uint8_t Mask>
    struct pin_mode_cfg : zoal::ct::type_list<typename Port::DDRx::template cas<0, 0>, typename Port::PORTx::template cas<0, 0>> {};

    template<class Port, uint8_t Mask>
    struct pin_mode_cfg<Port, ::zoal::gpio::pin_mode::input_floating, Mask>
        : zoal::ct::type_list<typename Port::DDRx::template cas<Mask, 0>, typename Port::PORTx::template cas<Mask, 0>> {};

    template<class Port, uint8_t Mask>
    struct pin_mode_cfg<Port, ::zoal::gpio::pin_mode::input_pull_up, Mask>
        : zoal::ct::type_list<typename Port::DDRx::template cas<Mask, 0>, typename Port::PORTx::template cas<0, Mask>> {};

    template<class Port, uint8_t Mask>
    struct pin_mode_cfg<Port, ::zoal::gpio::pin_mode::output_push_pull, Mask>
        : zoal::ct::type_list<typename Port::DDRx::template cas<0, Mask>, typename Port::PORTx::template cas<0, 0>> {};

    template<uintptr_t Address, uint8_t PinMask = 0xFF>
    class port {
    public:
        using self_type = port<Address, PinMask>;
        using register_type = uint8_t;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr auto address = Address;
        static constexpr uint8_t pin_mask = PinMask;

        using PINx = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, register_type, PinMask>;
        using DDRx = zoal::mem::reg<Address + 0x01, zoal::mem::reg_io::read_write, register_type, PinMask>;
        using PORTx = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, register_type, PinMask>;

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        using mode_cas = pin_mode_cfg<self_type, PinMode, Mask>;

        template<register_type Mask>
        using low_cas = zoal::ct::type_list<typename PORTx::template cas<Mask, 0>>;

        template<register_type Mask>
        using high_cas = zoal::ct::type_list<typename PORTx::template cas<0, Mask>>;

        ZOAL_INLINE_IO static void power_on() {}

        ZOAL_INLINE_IO static void power_off() {
            mode<::zoal::gpio::pin_mode::input_floating, PinMask>();
        }

        ZOAL_INLINE_IO static register_type read() {
            return PINx::ref();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void low() {
            zoal::mem::apply_cas_list<low_cas<Mask>>();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void high() {
            zoal::mem::apply_cas_list<high_cas<Mask>>();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void toggle() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            PORTx::ref() ^= Mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        ZOAL_INLINE_IO static void mode() {
            zoal::mem::apply_cas_list<mode_cas<PinMode, Mask>>();
        }
    };
}}}

#endif

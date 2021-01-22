#ifndef ZOAL_ARCH_AVR_PORT_HPP
#define ZOAL_ARCH_AVR_PORT_HPP

#include "../../ct/type_list.hpp"
#include "../../gpio/pin_mode.hpp"
#include "../../mem/reg.hpp"
#include "../../utils/defs.hpp"
#include "../bus.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    using zoal::mem::callable_cas_list_variadic;

    template<class Port, ::zoal::gpio::pin_mode PinMode, uint8_t Mask>
    struct pin_mode_cfg : callable_cas_list_variadic<typename Port::DDRx::template cas<0, 0>> {};

    template<class Port, uint8_t Mask>
    struct pin_mode_cfg<Port, ::zoal::gpio::pin_mode::input_floating, Mask>
        : callable_cas_list_variadic<typename Port::DDRx::template cas<Mask, 0>, typename Port::PORTx::template cas<Mask, 0>> {};

    template<class Port, uint8_t Mask>
    struct pin_mode_cfg<Port, ::zoal::gpio::pin_mode::input_pull_up, Mask>
        : callable_cas_list_variadic<typename Port::DDRx::template cas<Mask, 0>, typename Port::PORTx::template cas<0, Mask>> {};

    template<class Port, uint8_t Mask>
    struct pin_mode_cfg<Port, ::zoal::gpio::pin_mode::output_push_pull, Mask>
        : callable_cas_list_variadic<typename Port::DDRx::template cas<0, Mask>> {};

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

        using clock_on_cas = zoal::mem::null_cas_list;
        using clock_off_cas = mode_cas<::zoal::gpio::pin_mode::input_floating, PinMask>;

        template<register_type Mask>
        using low_cas = callable_cas_list_variadic<typename PORTx::template cas<Mask, 0>>;

        template<register_type Mask>
        using high_cas = callable_cas_list_variadic<typename PORTx::template cas<0, Mask>>;

        ZOAL_INLINE_IO static register_type read() {
            return PINx::ref();
        }

        template<register_type Mask>
        ZOAL_INLINE_IO static void toggle() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            PORTx::ref() ^= Mask;
        }
    };
}}}

#endif

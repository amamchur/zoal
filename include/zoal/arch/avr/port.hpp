#ifndef ZOAL_ARCH_AVR_PORT_HPP
#define ZOAL_ARCH_AVR_PORT_HPP

#include "../../gpio/base_port.hpp"
#include "../../gpio/pin_mode.hpp"
#include "../../utils/memory_segment.hpp"
#include "../../utils/helpers.hpp"
#include "../bus.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template <::zoal::gpio::pin_mode PinMode, uint8_t Mask>
    struct pin_mode_cfg {
        using DDRx = ::zoal::utils::clear_and_set<0, 0>;
        using PORTx = ::zoal::utils::clear_and_set<0, 0>;
    };

    template <uint8_t Mask>
    struct pin_mode_cfg<::zoal::gpio::pin_mode::input_floating, Mask> {
        using DDRx = ::zoal::utils::clear_and_set<Mask, 0>;
        using PORTx = ::zoal::utils::clear_and_set<Mask, 0>;
    };

    template <uint8_t Mask>
    struct pin_mode_cfg<::zoal::gpio::pin_mode::input_pull_up, Mask> {
        using DDRx = ::zoal::utils::clear_and_set<Mask, 0>;
        using PORTx = ::zoal::utils::clear_and_set<0, Mask>;
    };

    template <uint8_t Mask>
    struct pin_mode_cfg<::zoal::gpio::pin_mode::output_push_pull, Mask> {
        using DDRx = ::zoal::utils::clear_and_set<0, Mask>;
        using PORTx = ::zoal::utils::clear_and_set<0, 0>;
    };

    template<uintptr_t Address, uint8_t PinMask = 0xFF>
    class port : public zoal::gpio::base_port<uint8_t, Address, PinMask> {
    public:
        using base_type = zoal::gpio::base_port<uint8_t, Address, PinMask>;
        using self_type = port<Address, PinMask>;
        using register_type = typename base_type::register_type;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uint8_t pin_mask = PinMask;
        static constexpr uintptr_t PINx = 0x00;
        static constexpr uintptr_t DDRx = 0x01;
        static constexpr uintptr_t PORTx = 0x02;

        port() = delete;

        static void power_on() {}

        static void power_off() {
            mode<::zoal::gpio::pin_mode::input_floating>(PinMask);
        }

        static register_type read() {
            return mem[PINx];
        }

        template<register_type Mask>
        static void low() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[PORTx] &= ~Mask;
        }

        template<register_type Mask>
        static void high() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[PORTx] |= Mask;
        }

        template<register_type Mask>
        static void toggle() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            mem[PORTx] ^= Mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        static void mode() {
            static_assert((Mask & pin_mask) == Mask && Mask != 0, "Incorrect pin mask");
            using cfg = pin_mode_cfg<PinMode, Mask & pin_mask>;
            constexpr auto changes = cfg::DDRx::clear_mask
                    | cfg::DDRx::set_mask
                    | cfg::PORTx::clear_mask
                    | cfg::PORTx::set_mask;
            static_assert(changes != 0, "Unsupported pin mode");
            cfg::DDRx::apply(mem[DDRx]);
            cfg::PORTx::apply(mem[PORTx]);
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t PinMask>
    zoal::utils::memory_segment<uint8_t, Address> port<Address, PinMask>::mem;
}}}

#endif

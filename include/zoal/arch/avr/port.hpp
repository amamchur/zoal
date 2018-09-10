#ifndef ZOAL_ARCH_AVR_PORT_HPP
#define ZOAL_ARCH_AVR_PORT_HPP

#include "../../gpio/base_port.hpp"
#include "../../gpio/pin_mode.hpp"
#include "../../utils/memory_segment.hpp"
#include "../bus.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t PinMask = 0xFF>
    class port : public zoal::gpio::base_port<uint8_t, Address, PinMask> {
    public:
        using base_type = zoal::gpio::base_port<uint8_t, Address, PinMask>;
        using self_type = port<Address, PinMask>;
        using register_type = typename base_type::register_type;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t PINx = 0x00;
        static constexpr uintptr_t DDRx = 0x01;
        static constexpr uintptr_t PORTx = 0x02;

        port() = delete;

        static void power_on() {}

        static void power_off() {
            mode<::zoal::gpio::pin_mode::input_floating>(0xFF);
        }

        static register_type read() {
            return mem[PINx];
        }

        static void low(register_type mask) {
            mem[PORTx] &= ~mask;
        }

        static void high(register_type mask) {
            mem[PORTx] |= mask;
        }

        static void toggle(register_type mask) {
            mem[PORTx] ^= mask;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type mask>
        static void mode() {
            using namespace ::zoal::gpio;
            switch (PinMode) {
            case pin_mode::input_floating:
            case pin_mode::input_pull_down:
                mem[DDRx] &= ~mask;
                mem[PORTx] &= ~mask;
                break;
            case pin_mode::input_pull_up:
                mem[DDRx] &= ~mask;
                mem[PORTx] |= mask;
                break;
            case pin_mode::output_open_drain:
            case pin_mode::output_push_pull:
                mem[DDRx] |= mask;
                break;
            }
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t PinMask>
    zoal::utils::memory_segment<uint8_t, Address> port<Address, PinMask>::mem;
}}}

#endif

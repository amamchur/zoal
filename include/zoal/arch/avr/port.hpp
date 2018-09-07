#ifndef ZOAL_ARCH_AVR_PORT_HPP
#define ZOAL_ARCH_AVR_PORT_HPP

#include <stdint.h> /* NOLINT */

#include "../../gpio/pin_mode.hpp"
#include "../../utils/memory_segment.hpp"
#include "../bus.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class port {
    public:
        using self_type = port<Address>;
        using register_type = uint8_t;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t address = Address;
        static constexpr uintptr_t PINx = 0x00;
        static constexpr uintptr_t DDRx = 0x01;
        static constexpr uintptr_t PORTx = 0x02;

        port() = delete;

        static inline void power_on() {}

        static inline void power_off() {
            mode<::zoal::gpio::pin_mode::input_floating>(0xFF);
        }

        static inline register_type read() {
            return mem[PINx];
        }

        static inline void low(register_type mask) {
            mem[PORTx] &= ~mask;
        }

        static inline void high(register_type mask) {
            mem[PORTx] |= mask;
        }

        static inline void toggle(register_type mask) {
            mem[PORTx] ^= mask;
        }

        template<::zoal::gpio::pin_mode PinMode>
        static inline void mode(register_type mask) {
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

        template<::zoal::gpio::pin_mode PinMode, register_type mask>
        static inline void mode() {
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

    template<uintptr_t Address>
    zoal::utils::memory_segment<uint8_t, Address> port<Address>::mem;
}}}

#endif

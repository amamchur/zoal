#ifndef ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP
#define ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP

#include "zoal/mem/segment.hpp"
#include "../bus.hpp"
#include "mcu_type.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, class... Mixin>
    class adc : public Mixin... {
    public:
        using self_type = adc<Address, N, Mixin...>;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t resolution = 10;

        adc() = delete;

        adc(const adc &) = delete;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            mem[self_type::ADCSRAx] |= static_cast<uint8_t>(1u << 7u);
        }

        static void disable() {
            mem[self_type::ADCSRAx] &= ~static_cast<uint8_t>(1u << 7u);
        }

        static void enable_interrupt() {
            mem[self_type::ADCSRAx] |= 0x08;
        }

        static void disable_interrupt() {
            mem[self_type::ADCSRAx] &= ~0x08;
        }

        static void start() {
            mem[self_type::ADCSRAx] |= 1 << 6;
        }

        static void wait() {
            while (mem[self_type::ADCSRAx] & (1 << 6)) {
            }
        }

        static uint16_t value() {
            zoal::mem::segment<uint16_t, Address> m;
            return m[self_type::ADCx];
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }

    private:
        static zoal::mem::segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, class... Mixin>
    zoal::mem::segment<uint8_t, Address> adc<Address, N, Mixin...>::mem;
}}}

#endif

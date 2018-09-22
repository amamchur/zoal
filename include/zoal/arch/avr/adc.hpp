#ifndef ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP
#define ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP

#include "../../mem/accessor.hpp"
#include "../bus.hpp"
#include "mcu_type.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, class... Mixin>
    class adc : public Mixin... {
    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

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
            *accessor<self_type::ADCSRAx>::p |= static_cast<uint8_t>(1u << 7u);
        }

        static void disable() {
            *accessor<self_type::ADCSRAx>::p &= ~static_cast<uint8_t>(1u << 7u);
        }

        static void enable_interrupt() {
            *accessor<self_type::ADCSRAx>::p |= 0x08;
        }

        static void disable_interrupt() {
            *accessor<self_type::ADCSRAx>::p &= ~0x08;
        }

        static void start() {
            *accessor<self_type::ADCSRAx>::p |= 1 << 6;
        }

        static void wait() {
            while (*accessor<self_type::ADCSRAx>::p & (1 << 6)) continue;
        }

        static uint16_t value() {
            return *zoal::mem::accessor<uint16_t, Address, self_type::ADCx>::p;
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP
#define ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP

#include "../../ct/type_list.hpp"
#include "../../mem/cas.hpp"
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

        using enable_cas = zoal::mem::cas_list<typename self_type::ADCSRAx::template cas<0, 1u << 7u>>;
        using disable_cas =  zoal::mem::cas_list<typename self_type::ADCSRAx::template cas<1u << 7u, 0>>;
        using clock_on_cas = zoal::mem::null_cas_list;
        using clock_off_cas = zoal::mem::null_cas_list;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            self_type::ADCSRAx::ref() |= static_cast<uint8_t>(1u << 7u);
        }

        static void disable() {
            self_type::ADCSRAx::ref() &= ~static_cast<uint8_t>(1u << 7u);
        }

        static void enable_interrupt() {
            self_type::ADCSRAx::ref() |= 0x08;
        }

        static void disable_interrupt() {
            self_type::ADCSRAx::ref() &= ~0x08;
        }

        static void start() {
            self_type::ADCSRAx::ref() |= 1 << 6;
        }

        static void wait() {
            while (self_type::ADCSRAx::ref() & (1 << 6)) continue;
        }

        static uint16_t value() {
            return self_type::ADCx::ref();
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }
    };
}}}

#endif

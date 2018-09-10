#ifndef ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP
#define ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP

#include <stdint.h> /* NOLINT */
#include "mcu_type.hpp"
#include "adc_model.hpp"
#include "../../utils/memory_segment.hpp"
#include "../bus.hpp"

namespace zoal { namespace arch { namespace avr {
    template<mcu_type Type, uintptr_t Address, uint8_t N>
    class adc {
    public:
        using model = adc_model<Type>;

        static constexpr zoal::arch::bus bus = zoal::arch::bus::common;
        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t resolution = 10;

        static constexpr uintptr_t ADCLx = model::ADCLx;
        static constexpr uintptr_t ADCHx = model::ADCHx;
        static constexpr uintptr_t ADCSRAx = model::ADCSRAx;
        static constexpr uintptr_t ADCSRBx = model::ADCSRBx;
        static constexpr uintptr_t ADMUXx = model::ADMUXx;

        adc() = delete;

        adc(const adc &) = delete;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            mem[model::ADCSRAx] |= static_cast<uint8_t >(1u << 7u);
        }

        static void disable() {
            mem[model::ADCSRAx] &= ~static_cast<uint8_t >(1u << 7u);
        }

        static void enable_interrupt() {
            mem[model::ADCSRAx] |= 0x08;
        }

        static void disable_interrupt() {
            mem[model::ADCSRAx] &= ~0x08;
        }

        static void start() {
            mem[model::ADCSRAx] |= 1 << 6;
        }

        static void wait() {
            while (mem[model::ADCSRAx] & (1 << 6));
        }

        static uint16_t value() {
            uint8_t low = mem[model::ADCLx];
            uint8_t high = mem[model::ADCHx];
            uint16_t result = high;
            result <<= 8;
            result |= low;
            return result;
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }
    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<mcu_type Type, uintptr_t Address, uint8_t N>
    zoal::utils::memory_segment<uint8_t, Address> adc<Type, Address, N>::mem;
}}}

#endif

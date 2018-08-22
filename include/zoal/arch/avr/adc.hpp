#ifndef ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP
#define ZOAL_ARCH_ATMEL_AVR_A2D_Converter_HPP

#include <stdint.h> /* NOLINT */
#include "mcu_type.hpp"
#include "adc_model.hpp"
#include "../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<mcu_type Type, uintptr_t Address, uint8_t N>
    class adc {
    public:
        using model = adc_model<Type>;

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t resolution = 10;

        adc() = delete;

        adc(const adc &) = delete;

        static inline void enable() {
            mem[model::ADCSRAx] = 0x87;
        }

        static inline void disable() {
            mem[model::ADCSRAx] &= ~static_cast<uint8_t >(1u << 7u);
        }

        static inline void enable_interrupt() {
            mem[model::ADCSRAx] |= 0x08;
        }

        static inline void disable_interrupt() {
            mem[model::ADCSRAx] &= ~0x08;
        }

        template<class Config>
        static void connect() {
            const uint8_t resetBits = (Config::extra >> 0) & 0xFF;
            const uint8_t setBits = (Config::extra >> 8) & 0xFF;

            if (Config::extra != 0) {
                mem[model::ADCSRBx] = (mem[model::ADCSRBx] & ~resetBits) | setBits;
            }

            mem[model::ADMUXx] = model::ADMUXxDef | (Config::channel & 0x07);
        }

        template<class Config>
        static void disconnect() {
        }

        static inline void start() {
            mem[model::ADCSRAx] |= 1 << 6;
        }

        static inline void wait() {
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

        static inline void setup() {
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<mcu_type Type, uintptr_t Address, uint8_t N>
    zoal::utils::memory_segment<uint8_t, Address> adc<Type, Address, N>::mem;
}}}

#endif

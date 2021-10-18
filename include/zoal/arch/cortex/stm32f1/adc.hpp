#ifndef ZOAL_ARCH_STM32F1_A2D_CONVERTER_HPP
#define ZOAL_ARCH_STM32F1_A2D_CONVERTER_HPP

#include "../../../mem/reg.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class... Mixin>
    class adc : public Mixin... {
    public:
        using self_type = adc<Address, Mixin...>;
        static constexpr uintptr_t address = Address;

        using ADCx_SR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_CR1 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_CR2 = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SMPR1 = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SMPR2 = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JOFR1 = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JOFR2 = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JOFR3 = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JOFR4 = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_HTR = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_LTR = zoal::mem::reg<Address + 0x28, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR1 = zoal::mem::reg<Address + 0x2C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR2 = zoal::mem::reg<Address + 0x30, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR3 = zoal::mem::reg<Address + 0x34, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JSQR = zoal::mem::reg<Address + 0x38, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JDR1 = zoal::mem::reg<Address + 0x3C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JDR2 = zoal::mem::reg<Address + 0x40, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JDR3 = zoal::mem::reg<Address + 0x44, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JDR4 = zoal::mem::reg<Address + 0x48, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_DR = zoal::mem::reg<Address + 0x4C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static constexpr uint32_t ADCx_CR2_SWSTART = 1 << 22;
        static constexpr uint32_t ADCx_CR2_EXTTRIG = 1 << 20;
        static constexpr uint32_t ADCx_CR2_ADON = 1 << 0;
        static constexpr uint32_t ADCx_SR_EOC = 1 << 1;

        static inline void enable() {
            ADCx_CR2::ref() |= ADCx_CR2_ADON;
        }

        static inline void disable() {
            ADCx_CR2::ref() &= ~ADCx_CR2_ADON;
        }

        static void start() {
            ADCx_CR2::ref() |= ADCx_CR2_SWSTART | ADCx_CR2_EXTTRIG | ADCx_CR2_ADON;
        }

        static inline void wait() {
            while ((ADCx_SR::ref() & ADCx_SR_EOC) != ADCx_SR_EOC) {
            }

            adc::ADCx_SR::ref() &= ~ADCx_SR_EOC;
        }

        static uint32_t read() {
            return ADCx_DR::ref();
        }
    };
}}}

#endif

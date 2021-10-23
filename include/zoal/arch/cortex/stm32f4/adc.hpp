#ifndef ZOAL_ARCH_STM32F1_A2D_Converter_HPP
#define ZOAL_ARCH_STM32F1_A2D_Converter_HPP

#include "../../../mem/reg.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f4 {
    template<uintptr_t Address, class... Mixin>
    class adc : public Mixin... {
    private:
        enum A2DC_Flags : uint32_t { Enable = 0x00000001, StartRegularConversion = 0x00000004 };

    public:
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

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t resolution = 12;

//        using enable_cas = zoal::ct::type_list<typename ADCx_CR::template cas<0, Enable>>;
//        using disable_cas = zoal::ct::type_list<typename ADCx_CR::template cas<Enable, 0>>;

        static void enable() {
//            ADCx_CR::ref() |= Enable;
        }

        static void disable() {
//            ADCx_CR::ref() &= ~Enable;
        }

        static inline void start() {
//            ADCx_CR::ref() |= Enable | StartRegularConversion;
        }

        static inline void wait() {
//            while ((ADCx_ISR::ref() & (1 << 2)) == 0) continue;
        }

        static uint16_t value() {
            return static_cast<uint16_t>(ADCx_DR::ref());
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }

        static void setup() {}
    };
}}}

#endif

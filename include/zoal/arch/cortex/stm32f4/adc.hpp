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
        using ADCx_ISR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_IER = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_CR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_CFGR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SMPR1 = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SMPR2 = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_TR1 = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_TR2 = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_TR3 = zoal::mem::reg<Address + 0x28, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR1 = zoal::mem::reg<Address + 0x30, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR2 = zoal::mem::reg<Address + 0x34, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR3 = zoal::mem::reg<Address + 0x38, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_SQR4 = zoal::mem::reg<Address + 0x3C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_DR = zoal::mem::reg<Address + 0x40, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_JSQR = zoal::mem::reg<Address + 0x4C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static constexpr uintptr_t ADCx_OFR1 = 0x60;
        static constexpr uintptr_t ADCx_OFR2 = 0x64;
        static constexpr uintptr_t ADCx_OFR3 = 0x68;
        static constexpr uintptr_t ADCx_OFR4 = 0x6C;
        static constexpr uintptr_t ADCx_JDR1 = 0x80;
        static constexpr uintptr_t ADCx_JDR2 = 0x84;
        static constexpr uintptr_t ADCx_JDR3 = 0x88;
        static constexpr uintptr_t ADCx_JDR4 = 0x8C;
        static constexpr uintptr_t ADCx_AWD2CR = 0xA0;
        static constexpr uintptr_t ADCx_AWD3CR = 0xA4;
        static constexpr uintptr_t ADCx_DIFSEL = 0xB0;
        static constexpr uintptr_t ADCx_CALFACT = 0xB4;

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t resolution = 12;

        static void enable() {
            ADCx_CR::ref() |= Enable;
        }

        static void disable() {
            ADCx_CR::ref() &= ~Enable;
        }

        template<class Config>
        static void connect() {
            auto vSQR1 = ADCx_SQR1::ref();
            vSQR1 &= ~(0x1F << 6);
            vSQR1 |= Config::channel << 6;
            ADCx_SQR1::ref() = vSQR1;

            if (Config::channel > 9) {
                auto vSMPR2 = ADCx_SMPR1::ref();
                vSMPR2 &= ~static_cast<uint32_t>(0x07u << (0x03u * (Config::channel - 10)));
                vSMPR2 |= static_cast<uint32_t>(0x03u << (0x03u * Config::channel));
                ADCx_SMPR2::ref() = vSMPR2;
            } else {
                auto vSMPR1 = ADCx_SMPR1::ref();
                vSMPR1 &= ~(0x38 << (3 * (Config::channel - 1)));
                vSMPR1 |= (uint32_t)0x03 << (3 * Config::channel);
                ADCx_SMPR1::ref() = vSMPR1;
            }
        }

        static inline void start() {
            ADCx_CR::ref() |= Enable | StartRegularConversion;
        }

        static inline void wait() {
            while ((ADCx_ISR::ref() & (1 << 2)) == 0) continue;
        }

        static uint16_t value() {
            return static_cast<uint16_t>(ADCx_DR::ref());
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }

        static void setup() {
        }
    };
}}}

#endif

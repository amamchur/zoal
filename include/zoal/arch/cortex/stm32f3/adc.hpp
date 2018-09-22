#ifndef ZOAL_ARCH_STM32F1_A2D_Converter_HPP
#define ZOAL_ARCH_STM32F1_A2D_Converter_HPP

#include "../../../mem/accessor.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f3 {
    template<uintptr_t Address, uint8_t N, class Clock>
    class adc : public Clock {
    private:
        enum A2DC_Flags : uint32_t { Enable = 0x00000001, StartRegularConversion = 0x00000004 };

    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint32_t, Address, Offset>;

        static constexpr uintptr_t ADCx_ISR = 0x00;
        static constexpr uintptr_t ADCx_IER = 0x04;
        static constexpr uintptr_t ADCx_CR = 0x08;
        static constexpr uintptr_t ADCx_CFGR = 0x0C;
        static constexpr uintptr_t ADCx_SMPR1 = 0x14;
        static constexpr uintptr_t ADCx_SMPR2 = 0x18;
        static constexpr uintptr_t ADCx_TR1 = 0x20;
        static constexpr uintptr_t ADCx_TR2 = 0x24;
        static constexpr uintptr_t ADCx_TR3 = 0x28;
        static constexpr uintptr_t ADCx_SQR1 = 0x30;
        static constexpr uintptr_t ADCx_SQR2 = 0x34;
        static constexpr uintptr_t ADCx_SQR3 = 0x38;
        static constexpr uintptr_t ADCx_SQR4 = 0x3C;
        static constexpr uintptr_t ADCx_DR = 0x40;
        static constexpr uintptr_t ADCx_JSQR = 0x4C;

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
        static constexpr uint8_t no = N;
        static constexpr uint8_t resolution = 12;

        static void enable() {
            *accessor<ADCx_CR>::p |= Enable;
        }

        static void disable() {
            *accessor<ADCx_CR>::p &= ~Enable;
        }

        template<class Config>
        static void connect() {
            auto vSQR1 = *accessor<ADCx_SQR1>::p;
            vSQR1 &= ~(0x1F << 6);
            vSQR1 |= Config::channel << 6;
            *accessor<ADCx_SQR1>::p = vSQR1;

            if (Config::channel > 9) {
                auto vSMPR2 = *accessor<ADCx_SMPR1>::p;
                vSMPR2 &= ~static_cast<uint32_t>(0x07u << (0x03u * (Config::channel - 10)));
                vSMPR2 |= static_cast<uint32_t>(0x03u << (0x03u * Config::channel));
                *accessor<ADCx_SMPR2>::p = vSMPR2;
            } else {
                auto vSMPR1 = *accessor<ADCx_SMPR1>::p;
                vSMPR1 &= ~(0x38 << (3 * (Config::channel - 1)));
                vSMPR1 |= (uint32_t)0x03 << (3 * Config::channel);
                *accessor<ADCx_SMPR1>::p = vSMPR1;
            }
        }

        static inline void start() {
            *accessor<ADCx_CR>::p |= Enable | StartRegularConversion;
        }

        static inline void wait() {
            while ((*accessor<ADCx_ISR>::p & (1 << 2)) == 0) continue;
        }

        static uint16_t value() {
            return static_cast<uint16_t>(*accessor<ADCx_DR>::p);
        }

        static uint16_t read() {
            start();
            wait();
            return value();
        }

        static void setup() {
            //            CommRegs::reset();
            //
            //            *accessor<ADCx_CFGR>::p = 0x00002000; // Single conversion mode
            //            *accessor<ADCx_SQR1>::p = 0x00000000; // Reset to defaults
            //            *accessor<ADCx_CR>::p |= Enable;
            //
            //            while ((*accessor<ADCx_ISR>::p & 0x0001) == 0)
            //                ;
        }
    };
}}}

#endif

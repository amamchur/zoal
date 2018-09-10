#ifndef ZOAL_MCU_METADATA_ATMEGA_16_32_U4_HPP
#define ZOAL_MCU_METADATA_ATMEGA_16_32_U4_HPP

#include "atmega.hpp"

namespace zoal { namespace metadata {
    template<int Tx, int Rx, int Ck>
    struct atmega_base_usart_mapping {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct atmega_usart_mapping : public atmega_base_usart_mapping<-1, -1, -1> {
    };

    template<>
    struct atmega_usart_mapping<0xC8, 0x0000, 0> : public atmega_base_usart_mapping<-1, -1, 0xFF> {
    };

    template<>
    struct atmega_usart_mapping<0xC8, 0x0029, 2> : public atmega_base_usart_mapping<-1, 0, -1> {
    };

    template<>
    struct atmega_usart_mapping<0xC8, 0x0029, 3> : public atmega_base_usart_mapping<0, -1, -1> {
    };


    template<uintptr_t AdcAddress, uintptr_t PortAddress, uint8_t PinOffset>
    struct atmega_pin_to_adc_channel {
        static constexpr int channel = -1;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x002f, 0> {
        static constexpr int channel = 0;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x002f, 1> {
        static constexpr int channel = 1;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x002f, 4> {
        static constexpr int channel = 2;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x002f, 5> {
        static constexpr int channel = 3;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x002f, 6> {
        static constexpr int channel = 4;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x002f, 7> {
        static constexpr int channel = 5;
    };

    template<class Adc, class Pin>
    struct atmega_adc_mapping : atmega_pin_to_adc_channel<Adc::address, Pin::port::address, Pin::offset> {
    };
}}

#endif
#ifndef ZOAL_MCU_METADATA_ATMEGA_48_88_168_328_HPP
#define ZOAL_MCU_METADATA_ATMEGA_48_88_168_328_HPP

#include "atmega.hpp"

namespace zoal { namespace metadata {
    template<int Tx, int Rx, int Ck>
    struct atmega_base_usart_mapping {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uintptr_t UsartAddress, uint32_t PortAddress, uint8_t PinOffset>
    struct atmega_usart_mapping : public atmega_base_usart_mapping<-1, -1, -1> {
    };

    template<>
    struct atmega_usart_mapping<0xC0, 0x0000, 0> : public atmega_base_usart_mapping<-1, -1, 0xFF> {
    };

    template<>
    struct atmega_usart_mapping<0xC0, 0x0029, 0> : public atmega_base_usart_mapping<-1, 0, -1> {
    };

    template<>
    struct atmega_usart_mapping<0xC0, 0x0029, 1> : public atmega_base_usart_mapping<0, -1, -1> {
    };

    template<uintptr_t AdcAddress, uintptr_t PortAddress, uint8_t PinOffset>
    struct atmega_pin_to_adc_channel {
        static constexpr int channel = -1;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x0026, 0> {
        static constexpr int channel = 0;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x0026, 1> {
        static constexpr int channel = 1;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x0026, 2> {
        static constexpr int channel = 2;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x0026, 3> {
        static constexpr int channel = 3;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x0026, 4> {
        static constexpr int channel = 4;
    };

    template<>
    struct atmega_pin_to_adc_channel<0x0078, 0x0026, 5> {
        static constexpr int channel = 5;
    };

    template<class Adc, class Pin>
    struct atmega_adc_mapping : atmega_pin_to_adc_channel<Adc::address, Pin::port::address, Pin::offset> {
    };
}}

#endif

#ifndef ZOAL_METADATA_HPP
#define ZOAL_METADATA_HPP

#include <stdint.h>
#include "zoal/gpio/pin.hpp"
#include "zoal/utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<int Tx, int Rx, int Ck>
    struct usart_af {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uint8_t Usart, uint32_t Port, uint8_t PinOffset>
    struct stm32_usart_af_mapping : public usart_af<-1, -1, -1> {
    };

    template<>
    struct stm32_usart_af_mapping<1, 0x00000000, 0> : public usart_af<0xFF, 0xFF, 0xFF> {
    };

    template<> // PA8 -> USART1_CK
    struct stm32_usart_af_mapping<1, 0x48000000, 8> : public usart_af<-1, -1, 7> {
    };

    template<> // PA9 -> USART1_TX
    struct stm32_usart_af_mapping<1, 0x48000000, 9> : public usart_af<7, -1, -1> {
    };

    template<> // PA10 -> USART1_RX
    struct stm32_usart_af_mapping<1, 0x48000000, 10> : public usart_af<-1, 7, -1> {
    };
}}

#endif

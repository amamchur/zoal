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

    template<uint8_t UsartNo, uint32_t Port, uint8_t PinOffset>
    struct atmega_usart_mapping : public atmega_base_usart_mapping<-1, -1, -1> {
    };

    template<>
    struct atmega_usart_mapping<0, 0x0000, 0> : public atmega_base_usart_mapping<-1, -1, 0xFF> {
    };

    template<>
    struct atmega_usart_mapping<0, 0x0029, 0> : public atmega_base_usart_mapping<-1, 0, -1> {
    };

    template<>
    struct atmega_usart_mapping<0, 0x0029, 1> : public atmega_base_usart_mapping<0, -1, -1> {
    };
}}

#endif

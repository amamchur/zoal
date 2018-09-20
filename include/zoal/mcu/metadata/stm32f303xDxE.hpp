#ifndef ZOAL_MCU_METADATA_SMT32F3XX_HPP
#define ZOAL_MCU_METADATA_SMT32F3XX_HPP

#include <stdint.h>
#include "../../gpio/pin.hpp"
#include "../../utils/helpers.hpp"
#include "zoal/periph/usart.hpp"

namespace zoal { namespace metadata {
    template<int Tx, int Rx, int Ck>
    struct usart_af {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uint8_t UsartNo, uint32_t Port, uint8_t PinOffset>
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

    template<uint8_t No>
    struct stm32_default_usart_freq {
    };

    template<>
    struct stm32_default_usart_freq<1> : zoal::ct::integral_constant<uint32_t, 72000000> {
    };

    template<>
    struct stm32_default_usart_freq<2> : zoal::ct::integral_constant<uint32_t, 36000000> {
    };

    template<>
    struct stm32_default_usart_freq<3> : zoal::ct::integral_constant<uint32_t, 36000000> {
    };

    template<>
    struct stm32_default_usart_freq<4> : zoal::ct::integral_constant<uint32_t, 36000000> {
    };

    template<>
    struct stm32_default_usart_freq<5> : zoal::ct::integral_constant<uint32_t, 36000000> {
    };


    template<uint8_t Bits>
    struct stm32_data_bits_to_cr1 {
    };

    template<>
    struct stm32_data_bits_to_cr1<7> {
        static const uint32_t set_mask = 0x10000000;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<>
    struct stm32_data_bits_to_cr1<8> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<>
    struct stm32_data_bits_to_cr1<9> {
        static const uint32_t set_mask = 0x1000;
        static const uint32_t clear_mask = 0x10001000;
    };


    template<zoal::periph::usart_parity Parity>
    struct stm32_parity_to_cr1 {
    };

    template<>
    struct stm32_parity_to_cr1<zoal::periph::usart_parity::none> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x600;
    };

    template<>
    struct stm32_parity_to_cr1<zoal::periph::usart_parity::even> {
        static const uint32_t set_mask = 0x400;
        static const uint32_t clear_mask = 0x600;
    };

    template<>
    struct stm32_parity_to_cr1<zoal::periph::usart_parity::odd> {
        static const uint32_t set_mask = 0x600;
        static const uint32_t clear_mask = 0x600;
    };

    template<zoal::periph::usart_stop_bits StopBit>
    struct stm32_stop_bits_to_cr2 {
    };

    template<>
    struct stm32_stop_bits_to_cr2<zoal::periph::usart_stop_bits::stop_bits_1> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x3000;
    };

    template<>
    struct stm32_stop_bits_to_cr2<zoal::periph::usart_stop_bits::stop_bits_2> {
        static const uint32_t set_mask = 0x2000;
        static const uint32_t clear_mask = 0x3000;
    };
}}

#endif

#ifndef ZOAL_MCU_METADATA_SMT32F3XX_HPP
#define ZOAL_MCU_METADATA_SMT32F3XX_HPP

#include "../../arch/bus.hpp"
#include "../../ct/constant.hpp"
#include "../../gpio/pin.hpp"
#include "../../periph/usart.hpp"
#include "../../utils/helpers.hpp"

#include <stdint.h>

namespace zoal { namespace metadata {
    template<int Tx, int Rx, int Ck>
    struct usart_af {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uintptr_t UsartNo, uintptr_t Port, uint8_t PinOffset>
    struct stm32_usart_af_mapping : public usart_af<-1, -1, -1> {};

    template<>
    struct stm32_usart_af_mapping<0x40013800u, 0x00000000, 0> : public usart_af<0xFF, 0xFF, 0xFF> {};

    template<> // PA8 -> USART1_CK
    struct stm32_usart_af_mapping<0x40013800u, 0x48000000, 8> : public usart_af<-1, -1, 7> {};

    template<> // PA9 -> USART1_TX
    struct stm32_usart_af_mapping<0x40013800u, 0x48000000, 9> : public usart_af<7, -1, -1> {};

    template<> // PA10 -> USART1_RX
    struct stm32_usart_af_mapping<0x40013800u, 0x48000000, 10> : public usart_af<-1, 7, -1> {};

    template<zoal::arch::bus Bus>
    struct stm32_bus_prescaler {};

    template<>
    struct stm32_bus_prescaler<zoal::arch::bus::cortex_ahb> : zoal::ct::integral_constant<uint8_t, 1> {};

    template<>
    struct stm32_bus_prescaler<zoal::arch::bus::cortex_apb1> : zoal::ct::integral_constant<uint8_t, 2> {};

    template<>
    struct stm32_bus_prescaler<zoal::arch::bus::cortex_apb2> : zoal::ct::integral_constant<uint8_t, 1> {};

    template<uint8_t Bits>
    struct stm32_data_bits_to_cr1 {};

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
    struct stm32_parity_to_cr1 {};

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
    struct stm32_stop_bits_to_cr2 {};

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

#ifndef ZOAL_MCU_METADATA_ATMEGA_HPP
#define ZOAL_MCU_METADATA_ATMEGA_HPP

#include "../../periph/usart_config.hpp"

namespace zoal { namespace metadata {
    template<::zoal::periph::usart_data_bits Bits>
    struct atmega_usart_data_bits_flags {
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_5> {
        static constexpr uint8_t flags = 0 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_6> {
        static constexpr uint8_t flags = 1 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_7> {
        static constexpr uint8_t flags = 2 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_8> {
        static constexpr uint8_t flags = 3 << 1;
    };

    template<::zoal::periph::usart_parity Parity>
    struct atmega_usart_parity_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct atmega_usart_parity_flags<::zoal::periph::usart_parity::even> {
        static constexpr uint8_t flags = 2 << 4;
    };

    template<>
    struct atmega_usart_parity_flags<::zoal::periph::usart_parity::odd> {
        static constexpr uint8_t flags = 3 << 4;
    };

    template<::zoal::periph::usart_stop_bits StopBits>
    struct atmega_usart_stop_bit_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct atmega_usart_stop_bit_flags<::zoal::periph::usart_stop_bits::stop_bits_2> {
        static constexpr uint8_t flags = 1 << 3;
    };


}}

#endif

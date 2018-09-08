#ifndef ZOAL_ARCH_AVR_ATMEGA_CFG_HPP
#define ZOAL_ARCH_AVR_ATMEGA_CFG_HPP

#include "../../../periph/usart_config.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace metadata {
    template<::zoal::periph::usart_data_bits Bits>
    struct atmega_usart_data_bits_flags;

    template<::zoal::periph::usart_parity Parity>
    struct atmega_usart_parity_flags;

    template<::zoal::periph::usart_stop_bits StopBits>
    struct atmega_usart_stop_bit_flags;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<zoal::periph::usart_data_bits Bits, zoal::periph::usart_parity Parity, zoal::periph::usart_stop_bits StopBits>
    struct atmega_usart_mode {
        static constexpr uint8_t UCSRxC_value = static_cast<uint8_t>(0) | zoal::metadata::atmega_usart_data_bits_flags<Bits>::flags |
                                                zoal::metadata::atmega_usart_parity_flags<Parity>::flags |
                                                zoal::metadata::atmega_usart_stop_bit_flags<StopBits>::flags;
    };

    template<class Api, uint32_t Frequency>
    class cfg {
    public:
        static constexpr auto frequency = Frequency;

        template<class U,
                 uint32_t BaudRate,
                 zoal::periph::usart_data_bits Bits = zoal::periph::usart_data_bits::data_bits_8,
                 zoal::periph::usart_parity Parity = zoal::periph::usart_parity::none,
                 zoal::periph::usart_stop_bits StopBits = zoal::periph::usart_stop_bits::stop_bits_1,
                 uint32_t Freq = frequency>
        class usart {
        public:
            static constexpr uint8_t UCSRxA_bit_U2X = 1 << 1;
            static constexpr auto value_x2 = (Freq / (8 * BaudRate)) - 1;
            static constexpr auto value_x1 = (Freq / (16 * BaudRate)) - 1;
            static constexpr auto value = value_x2 > 4095 ? value_x1 : value_x2;
            static constexpr uint8_t UCSRxA_value = static_cast<const uint8_t>(value_x2 > 4095 ? 0u : UCSRxA_bit_U2X);
            static constexpr uint8_t UBRRxL_value = static_cast<uint8_t>(value & 0xFFu);
            static constexpr uint8_t UBRRxH_value = static_cast<uint8_t>(value >> 0x8u);

            static void apply() {
                U::disable();

                zoal::utils::memory_segment<uint8_t, U::address> mem;
                mem[U::UBRRxL] = UBRRxL_value;
                mem[U::UBRRxH] = UBRRxH_value;
                mem[U::UCSRxA] = UCSRxA_value;
                mem[U::UCSRxC] = atmega_usart_mode<Bits, Parity, StopBits>::UCSRxC_value;
                mem.happyInspection();
            }
        };
    };
}}}}

#endif

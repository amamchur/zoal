#ifndef ZOAL_ARCH_STM32X_CFG_HPP
#define ZOAL_ARCH_STM32X_CFG_HPP

#include "../../../periph/usart_config.hpp"
#include "../../../utils/helpers.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace metadata {
    template<uint8_t No>
    struct stm32_default_usart_freq;
}}

namespace zoal { namespace arch { namespace stm32x {
    template<zoal::periph::usart_data_bits Bits>
    struct data_bits_to_cr1 {

    };

    template<>
    struct data_bits_to_cr1<zoal::periph::usart_data_bits::data_bits_7> {
        static const uint32_t set_mask = 0x10000000;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<>
    struct data_bits_to_cr1<zoal::periph::usart_data_bits::data_bits_8> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x10001000;
    };

    template<>
    struct data_bits_to_cr1<zoal::periph::usart_data_bits::data_bits_9> {
        static const uint32_t set_mask = 0x1000;
        static const uint32_t clear_mask = 0x10001000;
    };


    template<zoal::periph::usart_parity Parity>
    struct parity_to_cr1 {
    };

    template<>
    struct parity_to_cr1<zoal::periph::usart_parity::none> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x600;
    };

    template<>
    struct parity_to_cr1<zoal::periph::usart_parity::even> {
        static const uint32_t set_mask = 0x400;
        static const uint32_t clear_mask = 0x600;
    };

    template<>
    struct parity_to_cr1<zoal::periph::usart_parity::odd> {
        static const uint32_t set_mask = 0x600;
        static const uint32_t clear_mask = 0x600;
    };

    template<zoal::periph::usart_stop_bits StopBit>
    struct stop_bits_to_cr2 {
    };

    template<>
    struct stop_bits_to_cr2<zoal::periph::usart_stop_bits::stop_bits_1> {
        static const uint32_t set_mask = 0x0;
        static const uint32_t clear_mask = 0x3000;
    };

    template<>
    struct stop_bits_to_cr2<zoal::periph::usart_stop_bits::stop_bits_2> {
        static const uint32_t set_mask = 0x2000;
        static const uint32_t clear_mask = 0x3000;
    };

    template<class Api>
    class stm32x_cfg {
    public:
        template<
                class U,
                uint32_t BaudRate,
                zoal::periph::usart_data_bits Bits = zoal::periph::usart_data_bits::data_bits_8,
                zoal::periph::usart_parity Parity = zoal::periph::usart_parity::none,
                zoal::periph::usart_stop_bits StopBits = zoal::periph::usart_stop_bits::stop_bits_1,
                uint32_t Freq = zoal::metadata::stm32_default_usart_freq<U::no>::value
        >
        class usart {
        public:
            using dbc1 = data_bits_to_cr1<Bits>;
            using ptc1 = parity_to_cr1<Parity>;
            using sbc2 = stop_bits_to_cr2<StopBits>;

            static constexpr auto enable_rx_tx = 0x0C;
            static constexpr auto c1_clear = dbc1::clear_mask | ptc1::clear_mask;
            static constexpr auto c1_set = enable_rx_tx | dbc1::set_mask | ptc1::set_mask;
            static constexpr auto c2_clear = sbc2::clear_mask;
            static constexpr auto c2_set = sbc2::set_mask;

            static constexpr double divider = Freq / 16.0 / BaudRate;
            static constexpr auto int_part = static_cast<uint16_t>(divider);
            static constexpr auto mantissa = static_cast<uint16_t>((divider - int_part) * 16);
            static constexpr uint16_t bbr = (int_part << 4) + mantissa;

            static void apply() {
                using namespace zoal::utils;
                U::disable();

                zoal::utils::memory_segment<uint32_t, U::address> mem;
                mem[U::USARTx_BRR] = bbr;
                clear_and_set<c1_clear, c1_set, 0>::apply(mem[U::USARTx_CR1]);
                clear_and_set<c2_clear, c2_set, 0>::apply(mem[U::USARTx_CR2]);
                clear_and_set<0x300, 0, 0>::apply(mem[U::USARTx_CR3]);
            }
        };
    };
}}}

#endif

#ifndef ZOAL_ARCH_STM32X_CFG_HPP
#define ZOAL_ARCH_STM32X_CFG_HPP

#include "../../../periph/usart.hpp"
#include "../../../utils/helpers.hpp"
#include "../../bus.hpp"

namespace zoal { namespace metadata {
    template<zoal::arch::bus Bus>
    struct stm32_bus_prescaler;

    template<uint8_t Bits>
    struct stm32_data_bits_to_cr1;

    template<zoal::periph::usart_parity Parity>
    struct stm32_parity_to_cr1;

    template<zoal::periph::usart_stop_bits StopBit>
    struct stm32_stop_bits_to_cr2;
}}

namespace zoal { namespace arch { namespace stm32x {
    template<class Microcontroller>
    class cfg {
    public:
        using mcu = Microcontroller;
        using api = typename mcu::api;
        static constexpr auto mcu_frequency = mcu::frequency;

        template<class U,
                 uint32_t BaudRate,
                 uint8_t Bits = 8,
                 zoal::periph::usart_parity Parity = zoal::periph::usart_parity::none,
                 zoal::periph::usart_stop_bits StopBits = zoal::periph::usart_stop_bits::stop_bits_1,
                 uint32_t UsartFreq = mcu_frequency / zoal::metadata::stm32_bus_prescaler<U::bus>::value>
        class usart {
        public:
            using dbc1 = zoal::metadata::stm32_data_bits_to_cr1<Bits>;
            using ptc1 = zoal::metadata::stm32_parity_to_cr1<Parity>;
            using sbc2 = zoal::metadata::stm32_stop_bits_to_cr2<StopBits>;

            static constexpr auto enable_rx_tx = 0x0C;
            static constexpr auto c1_clear = dbc1::clear_mask | ptc1::clear_mask;
            static constexpr auto c1_set = enable_rx_tx | dbc1::set_mask | ptc1::set_mask;
            static constexpr auto c2_clear = sbc2::clear_mask;
            static constexpr auto c2_set = sbc2::set_mask;

            static constexpr double divider = UsartFreq / 16.0 / BaudRate;
            static constexpr auto int_part = static_cast<uint16_t>(divider);
            static constexpr auto mantissa = static_cast<uint16_t>((divider - int_part) * 16);
            static constexpr uint16_t bbr = (int_part << 4) + mantissa;

            using USARTx_CR1 = typename U::USARTx_CR1::template cas<c1_clear, c1_set>;
            using USARTx_CR2 = typename U::USARTx_CR2::template cas<c2_clear, c2_set>;
            using USARTx_CR3 = typename U::USARTx_CR3::template cas<0x300, 0>;
            using USARTx_BRR = typename U::USARTx_BRR::template cas<0, bbr>;

            static void apply() {
                U::disable();

                U::USARTx_BRR::ref() = bbr;
                USARTx_CR1();
                USARTx_CR2();
                USARTx_CR3();
                USARTx_BRR();
            }
        };
    };
}}}

#endif

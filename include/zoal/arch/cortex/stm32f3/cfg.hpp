#ifndef ZOAL_ARCH_STM32F3_CFG_HPP
#define ZOAL_ARCH_STM32F3_CFG_HPP

#include "../../../gpio/api.hpp"
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

namespace zoal { namespace arch { namespace stm32f3 {
    using zoal::ct::type_list;
    using zoal::gpio::api;

    template<class Microcontroller>
    class cfg {
    public:
        using mcu = Microcontroller;

        template<class U, class Cfg>
        class usart {
        public:
            using dbc1 = zoal::metadata::stm32_data_bits_to_cr1<Cfg::word_length_bits>;
            using ptc1 = zoal::metadata::stm32_parity_to_cr1<Cfg::parity>;
            using sbc2 = zoal::metadata::stm32_stop_bits_to_cr2<Cfg::stop_bits>;

            static constexpr auto enable_rx_tx = 0x0C;
            static constexpr auto c1_clear = dbc1::clear_mask | ptc1::clear_mask;
            static constexpr auto c1_set = enable_rx_tx | dbc1::set_mask | ptc1::set_mask;
            static constexpr auto c2_clear = sbc2::clear_mask;
            static constexpr auto c2_set = sbc2::set_mask;

            static constexpr uint16_t usart_div = static_cast<uint16_t>(static_cast<float>(Cfg::clock_frequency) / Cfg::baud_rate + 0.5);
            static constexpr uint16_t bbr = usart_div;

            static_assert(usart_div >= 0x10, "USARTDIV must be greater than or equal to 0d16");

            using USARTx_CR1 = typename U::USARTx_CR1::template cas<c1_clear, c1_set>;
            using USARTx_CR2 = typename U::USARTx_CR2::template cas<c2_clear, c2_set>;
            using USARTx_CR3 = typename U::USARTx_CR3::template cas<0x300, 0>;
            using USARTx_BRR = typename U::USARTx_BRR::template cas<0, bbr>;

            using periph_clock_on = typename api::optimize<typename U::clock_on_cas>;
            using apply = type_list<USARTx_CR1, USARTx_CR2, USARTx_CR3, USARTx_BRR>;
        };
    };
}}}

#endif

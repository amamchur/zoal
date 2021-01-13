#ifndef ZOAL_ARCH_STM32F1_CFG_HPP
#define ZOAL_ARCH_STM32F1_CFG_HPP

#include "../../../ct/type_list.hpp"
#include "../../../mem/cas.hpp"
#include "../../../periph/usart.hpp"
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

namespace zoal { namespace arch { namespace stm32f1 {
    using zoal::ct::type_list;

    template<class Microcontroller>
    class cfg {
    public:
        using mcu = Microcontroller;
        using api = typename mcu::api;

        template<class U, class Cfg>
        class usart {
        public:
            using dbc1 = zoal::metadata::stm32_data_bits_to_cr1<Cfg::word_length_bits>;
            using ptc1 = zoal::metadata::stm32_parity_to_cr1<Cfg::parity>;
            using sbc2 = zoal::metadata::stm32_stop_bits_to_cr2<Cfg::stop_bits>;

            static constexpr auto enable_rx_tx = U::USARTx_CR1_TE | U::USARTx_CR1_RE;
            static constexpr auto cr1_clear = dbc1::clear_mask | ptc1::clear_mask;
            static constexpr auto cr1_set = enable_rx_tx | dbc1::set_mask | ptc1::set_mask;
            static constexpr auto cr2_clear = sbc2::clear_mask;
            static constexpr auto cr2_set = sbc2::set_mask;

            static constexpr double divider = Cfg::clock_frequency / 16.0 / Cfg::baud_rate;
            static constexpr auto int_part = static_cast<uint16_t>(divider);
            static constexpr auto mantissa = static_cast<uint16_t>((divider - int_part) * 16);
            static constexpr uint16_t bbr_set = (int_part << 4) + mantissa;

            using USARTx_CR1 = typename U::USARTx_CR1::template cas<cr1_clear, cr1_set>;
            using USARTx_CR2 = typename U::USARTx_CR2::template cas<cr2_clear, cr2_set>;
            using USARTx_CR3 = typename U::USARTx_CR3::template cas<0x300, 0>;
            using USARTx_BRR = typename U::USARTx_BRR::template cas<0, bbr_set>;

            using apply = type_list<USARTx_CR1, USARTx_CR2, USARTx_CR3, USARTx_BRR>;
        };
    };
}}}

#endif

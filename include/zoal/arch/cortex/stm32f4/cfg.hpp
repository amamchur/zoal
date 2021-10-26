#ifndef ZOAL_ARCH_STM32F4_CFG_HPP
#define ZOAL_ARCH_STM32F4_CFG_HPP

#include "../../../ct/type_list.hpp"
#include "../../../gpio/api.hpp"
#include "../../../gpio/pin_mode.hpp"
#include "../../../periph/spi.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/helpers.hpp"
#include "../../bus.hpp"

namespace zoal { namespace metadata {
    template<uint8_t Bits>
    struct stm32_data_bits_to_cr1;

    template<zoal::periph::usart_parity Parity>
    struct stm32_parity_to_cr1;

    template<zoal::periph::usart_stop_bits StopBit>
    struct stm32_stop_bits_to_cr2;

    template<int Bits>
    struct stm32_spi_clock_div_to_cr1_br;
}}

namespace zoal { namespace arch { namespace stm32f4 {
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

            static constexpr double divider = Cfg::clock_frequency / 16.0 / Cfg::baud_rate;
            static constexpr auto int_part = static_cast<uint16_t>(divider);
            static constexpr auto mantissa = static_cast<uint16_t>((divider - int_part) * 16);
            static constexpr uint16_t bbr = (int_part << 4) + mantissa;

            using USARTx_CR1 = typename U::USARTx_CR1::template cas<c1_clear, c1_set>;
            using USARTx_CR2 = typename U::USARTx_CR2::template cas<c2_clear, c2_set>;
            using USARTx_CR3 = typename U::USARTx_CR3::template cas<0x300, 0>;
            using USARTx_BRR = typename U::USARTx_BRR::template cas<0, bbr>;

            using periph_clock_on = typename api::optimize<typename U::clock_on_cas>;
            using apply = zoal::ct::type_list<USARTx_CR1, USARTx_CR2, USARTx_CR3, USARTx_BRR>;
        };

        template<class S, class Cfg>
        class spi {
        public:
            using br_v = zoal::metadata::stm32_spi_clock_div_to_cr1_br<Cfg::clock_divider>;

            static_assert(br_v::value >= 0, "Unsupported clock divider");

            static constexpr uint32_t baud_rate = br_v::value << 3;
            static constexpr uint32_t lsbf = (Cfg::order == zoal::periph::bit_order::lsbf ? 1 : 0) << 7;
            static constexpr uint32_t master = (Cfg::mode == zoal::periph::spi_mode::master ? 1 : 0) << 2;
            static constexpr uint32_t ssm_ssi = (1 << 9) | (1 << 8);
            //            static constexpr uint32_t cpol = (Cfg::order == zoal::periph::bit_order::lsbf ? 1 : 0) << 7;
            //            static constexpr uint32_t cpha = (Cfg::order == zoal::periph::bit_order::lsbf ? 1 : 0) << 7;

            using USARTx_CR1 = typename S::SPIx_CR1::template cas<0xFFFF, baud_rate | lsbf | master | ssm_ssi>;

            using periph_clock_on = typename api::optimize<typename S::clock_on_cas>;
            using apply = zoal::ct::type_list<USARTx_CR1>;
        };
    };
}}}

#endif

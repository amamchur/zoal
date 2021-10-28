#ifndef ZOAL_ARCH_STM32F4_CFG_HPP
#define ZOAL_ARCH_STM32F4_CFG_HPP

#include "../../../ct/type_list.hpp"
#include "../../../ct/value_list.hpp"
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

        template<class U, class Params>
        class usart {
        public:
            using dbc1 = zoal::metadata::stm32_data_bits_to_cr1<Params::word_length_bits>;
            using ptc1 = zoal::metadata::stm32_parity_to_cr1<Params::parity>;
            using sbc2 = zoal::metadata::stm32_stop_bits_to_cr2<Params::stop_bits>;

            static constexpr auto enable_rx_tx = 0x0C;
            static constexpr auto c1_clear = dbc1::clear_mask | ptc1::clear_mask;
            static constexpr auto c1_set = enable_rx_tx | dbc1::set_mask | ptc1::set_mask;
            static constexpr auto c2_clear = sbc2::clear_mask;
            static constexpr auto c2_set = sbc2::set_mask;

            static constexpr double divider = Params::clock_frequency / 16.0 / Params::baud_rate;
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

        template<class S, class Params>
        class spi {
        public:
            using br_v = zoal::metadata::stm32_spi_clock_div_to_cr1_br<Params::clock_divider>;

            static_assert(br_v::value >= 0, "Unsupported clock divider");

            static constexpr uint32_t baud_rate = br_v::value << 3;
            static constexpr uint32_t lsbf = (Params::order == zoal::periph::bit_order::lsbf ? 1 : 0) << 7;
            static constexpr uint32_t master = (Params::mode == zoal::periph::spi_mode::master ? 1 : 0) << 2;
            static constexpr uint32_t ssm_ssi = (1 << 9) | (1 << 8);
            //            static constexpr uint32_t cpol = (Params::order == zoal::periph::bit_order::lsbf ? 1 : 0) << 7;
            //            static constexpr uint32_t cpha = (Params::order == zoal::periph::bit_order::lsbf ? 1 : 0) << 7;

            using USARTx_CR1 = typename S::SPIx_CR1::template cas<0xFFFF, baud_rate | lsbf | master | ssm_ssi>;

            using periph_clock_on = typename api::optimize<typename S::clock_on_cas>;
            using apply = zoal::ct::type_list<USARTx_CR1>;
        };

        template<class A, class Params>
        class adc {
        public:
            using adc_prescalers = ::zoal::ct::value_list<uint8_t, 2, 4, 6, 8>;
            static constexpr auto adc_pre = ::zoal::ct::index_of_value<uint8_t, Params::clock_divider, adc_prescalers>::result;

            static_assert(adc_pre >= 0, "Unsupported clock divider");

            using common_regs = typename A::common_regs;
            using ADCx_CR1 = typename A::ADCx_CR1::template cas<0xFFFFFFFF, 0>;
            using ADCx_CR2 = typename A::ADCx_CR2::template cas<0xFFFFFFFF, 1 << 10>;
            using ADCx_SQR1 = typename A::ADCx_SQR1::template cas<0xFFFFFF, 0>;
            using ADC_COMx_CCR = typename common_regs::ADC_COMx_CCR::template cas<0xC30000, adc_pre << 16>;

            using periph_clock_on = typename api::optimize<typename A::clock_on_cas>;
            using apply = zoal::ct::type_list<ADCx_CR1, ADCx_CR2, ADCx_SQR1, ADC_COMx_CCR>;
        };

        template<class T, class Params>
        class timer {
        public:
            using timer_prescalers = ::zoal::ct::value_list<uint8_t, 1, 2, 4>;
            static constexpr auto timer_pre = ::zoal::ct::index_of_value<uint8_t, Params::clock_divider, timer_prescalers>::result;
            static_assert(timer_pre >= 0, "Unsupported clock divider");

            using TIMERx_CR1 = typename T::TIMERx_CR1::template cas<0xFFFFFFFF, timer_pre << 8>;
            using TIMERx_CR2 = typename T::TIMERx_CR2::template cas<0xFFFFFFFF, 0>;
            using TIMERx_SMCR = typename T::TIMERx_SMCR::template cas<0xFFFFFFFF, 0>;
            using TIMERx_ARR = typename T::TIMERx_ARR::template cas<T::TIMERx_ARR::mask, Params::period>;
            using TIMERx_PSC = typename T::TIMERx_PSC::template cas<T::TIMERx_PSC::mask, Params::prescaler>;

            using periph_clock_on = typename api::optimize<typename T::clock_on_cas>;
            using apply = zoal::ct::type_list<TIMERx_CR1, TIMERx_CR2, TIMERx_SMCR, TIMERx_ARR, TIMERx_PSC>;
        };

        template<class I, class Params>
        class i2c {
        public:
            using periph_clock_on = typename api::optimize<typename I::clock_on_cas>;
            using apply = zoal::mem::null_cas_list;
        };
    };
}}}

#endif

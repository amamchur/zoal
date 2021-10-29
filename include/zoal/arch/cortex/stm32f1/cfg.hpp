#ifndef ZOAL_ARCH_STM32F1_CFG_HPP
#define ZOAL_ARCH_STM32F1_CFG_HPP

#include "../../../ct/type_list.hpp"
#include "../../../gpio/api.hpp"
#include "../../../mem/cas.hpp"
#include "../../../periph/usart.hpp"
#include "../../bus.hpp"

#include <zoal/periph/timer_mode.hpp>

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
    using zoal::gpio::api;
    using zoal::mem::callable_cas_list_variadic;

    template <zoal::periph::timer_mode M>
    class cr1_timer_mode {};

    template <>
    class cr1_timer_mode<zoal::periph::timer_mode::up> {
    public:
        static constexpr uint32_t clear = 0x70;
        static constexpr uint32_t set = 0;
    };

    template <>
    class cr1_timer_mode<zoal::periph::timer_mode::down> {
    public:
        static constexpr uint32_t clear = 0x70;
        static constexpr uint32_t set = 0x10;
    };

    template <>
    class cr1_timer_mode<zoal::periph::timer_mode::up_down> {
    public:
        static constexpr uint32_t clear = 0x70;
        static constexpr uint32_t set = 0x20;
    };

    template <uint32_t Prescaler>
    class cr1_timer_clock_source {
    };

    template <>
    class cr1_timer_clock_source<1> {
    public:
        static constexpr uint32_t clear = 0x0000;
        static constexpr uint32_t set = 0x0300;
    };

    template <>
    class cr1_timer_clock_source<2> {
    public:
        static constexpr uint32_t clear = 0x0100;
        static constexpr uint32_t set = 0x0300;
    };

    template <>
    class cr1_timer_clock_source<4> {
    public:
        static constexpr uint32_t clear = 0x0200;
        static constexpr uint32_t set = 0x0300;
    };

    template<class Microcontroller>
    class cfg {
    public:
        using mcu = Microcontroller;
        using sign = typename mcu::signature;

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

            using usart_clock_on = typename U::clock_on_cas;

            using USARTx_CR1 = typename U::USARTx_CR1::template cas<cr1_clear, cr1_set>;
            using USARTx_CR2 = typename U::USARTx_CR2::template cas<cr2_clear, cr2_set>;
            using USARTx_CR3 = typename U::USARTx_CR3::template cas<0x300, 0>;
            using USARTx_BRR = typename U::USARTx_BRR::template cas<0, bbr_set>;

            using clock_on = api::optimize<usart_clock_on>;
            using apply = type_list<USARTx_CR1, USARTx_CR2, USARTx_CR3, USARTx_BRR>;
        };

        template<class I, class Cfg>
        class i2c {
        public:
            //UM10204
            //I2C-bus specification and user manual. Page 48, table 10
            static constexpr auto max_rise_time_standard_mode_ns = 1000;
            static constexpr auto max_rise_time_fast_mode_ns = 300;

            static constexpr auto peripheral_clock = Cfg::clock_frequency;
            static constexpr auto peripheral_clock_mhz = peripheral_clock / 1000000;
            static constexpr auto i2c_frequency = Cfg::i2c_frequency;
            static constexpr bool i2c_standard_mode = i2c_frequency <= 100000;
            static constexpr auto max_rise_time = i2c_standard_mode ? (peripheral_clock_mhz * max_rise_time_standard_mode_ns) / 1000 + 1
                                                                    : (peripheral_clock_mhz * max_rise_time_fast_mode_ns) / 1000 + 1;
            static constexpr auto standard_mode_ccr = (peripheral_clock / i2c_frequency) / (1 + 1); // 1:1
            static constexpr auto fast_mode_duty_0_ccr = (peripheral_clock / i2c_frequency) / (2 + 1); // 2:1
            static constexpr auto fast_mode_duty_1_ccr = (peripheral_clock / i2c_frequency) / (16 + 9); // 16:9
            static constexpr auto duty_0_accuracy = peripheral_clock % (i2c_frequency * (1 + 2));
            static constexpr auto duty_1_accuracy = peripheral_clock % (i2c_frequency * (9 + 16));
            static constexpr auto duty = duty_0_accuracy < duty_1_accuracy ? 0 : 1;
            static constexpr auto ccr = i2c_standard_mode ? standard_mode_ccr : (duty == 0 ? fast_mode_duty_0_ccr : fast_mode_duty_1_ccr);

            using I2Cx_CR1 = typename I::I2Cx_CR1::template cas<0xC0, 0>; // clear NOSTRETCH & ENGC
            using I2Cx_CR2 = typename I::I2Cx_CR2::template cas<0x3F, peripheral_clock_mhz>;
            using I2Cx_OAR1 = typename I::I2Cx_OAR1::template cas<0xFFFFFFFF, 0>;
            using I2Cx_OAR2 = typename I::I2Cx_OAR2::template cas<0xFFFFFFFF, 0>;
            static constexpr auto I2Cx_CCR_FS = (i2c_standard_mode ? 0 : 1) << 15;
            static constexpr auto I2Cx_CCR_DUTY = duty << 14;
            static constexpr auto I2Cx_CCR_CCR = ccr;
            using I2Cx_CCR = typename I::I2Cx_CCR::template cas<0xFFFFFFFF, I2Cx_CCR_FS | I2Cx_CCR_DUTY | I2Cx_CCR_CCR>;
            using I2Cx_TRISE = typename I::I2Cx_TRISE::template cas<0x3F, max_rise_time>;

            using clock_on = api::optimize<typename I::clock_on_cas>;
            using apply = type_list<I2Cx_CR1, I2Cx_CR2, I2Cx_OAR1, I2Cx_OAR2, I2Cx_CCR, I2Cx_TRISE>;
        };

        template<class T, class Cfg>
        class timer {
        public:
            using cr1_mode = cr1_timer_mode<Cfg::mode>;
            using cr1_cs = cr1_timer_clock_source<Cfg::clock_divider>;

            using TIMERx_CR1 = typename T::TIMERx_CR1::template cas<cr1_mode::clear | cr1_cs::clear, cr1_mode::set | cr1_cs::set>;
            using TIMERx_ARR = typename T::TIMERx_ARR::template cas<T::TIMERx_ARR::mask, Cfg::period>;
            using TIMERx_PSC = typename T::TIMERx_PSC::template cas<T::TIMERx_PSC::mask, Cfg::prescaler>;
            using TIMERx_EGR = typename T::TIMERx_EGR::template cas<0xFF, T::TIMERx_EGR_UG>;
            using apply = type_list<TIMERx_CR1, TIMERx_PSC, TIMERx_ARR, TIMERx_EGR>;
        };
    };
}}}

#endif

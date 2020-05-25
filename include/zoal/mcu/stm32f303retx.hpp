/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_STM32F303RETX_HPP
#define ZOAL_MCU_STM32F303RETX_HPP

#include <stdint.h>
#include <zoal/arch/cortex/stm32f3/adc.hpp>
#include <zoal/arch/cortex/stm32f3/adc_common_regs.hpp>
#include <zoal/arch/cortex/stm32f3/general_purpose_timer.hpp>
#include <zoal/arch/cortex/stm32f3/spi.hpp>
#include <zoal/arch/cortex/stm32x/bus_clock.hpp>
#include <zoal/arch/cortex/stm32x/cfg.hpp>
#include <zoal/arch/cortex/stm32x/metadata.hpp>
#include <zoal/arch/cortex/stm32x/mux.hpp>
#include <zoal/arch/cortex/stm32x/port.hpp>
#include <zoal/arch/cortex/stm32x/rcc.hpp>
#include <zoal/arch/cortex/stm32x/usart.hpp>
#include <zoal/arch/enable.hpp>
#include <zoal/arch/power.hpp>
#include <zoal/gpio/api.hpp>

namespace zoal { namespace mcu {
    template<uint32_t HighSpeedExternalOscillator = 8000000, uint8_t PhaseLockedLoop = 9>
    class stm32f303retx {
    public:
        static constexpr auto hse = HighSpeedExternalOscillator;
        static constexpr auto pll = PhaseLockedLoop;
        static constexpr auto frequency = hse * pll;

        using self_type = stm32f303retx<hse, pll>;

        using rcc = ::zoal::arch::stm32x::rcc<>;

        template<uint32_t Mask>
        using clock_ahb = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb, Mask>;

        template<uint32_t Mask>
        using clock_apb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb1, Mask>;

        template<uint32_t Mask>
        using clock_apb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb2, Mask>;

        template<uintptr_t Address, uint8_t N, class Clock>
        using adc = typename ::zoal::arch::stm32x::adc<Address, N, Clock>;

        template<uintptr_t Address, class Clock, uint32_t PinMask>
        using port = typename ::zoal::arch::stm32x::port<Address, Clock, PinMask>;

        using port_a = port<0x48000000, clock_ahb<0x00020000>, 0xFFFF>;
        using port_b = port<0x48000400, clock_ahb<0x00040000>, 0xFFFF>;
        using port_c = port<0x48000800, clock_ahb<0x00080000>, 0xFFFF>;
        using port_d = port<0x48000C00, clock_ahb<0x00100000>, 0x0004>;
        using port_f = port<0x48001400, clock_ahb<0x00400000>, 0x0003>;

        using adc_01 = ::zoal::arch::stm32x::adc<0x50000000, 1, clock_ahb<0x10000000>>;
        using adc_02 = ::zoal::arch::stm32x::adc<0x50000100, 2, clock_ahb<0x10000000>>;
        using adc_03 = ::zoal::arch::stm32x::adc<0x50000400, 3, clock_ahb<0x20000000>>;
        using adc_04 = ::zoal::arch::stm32x::adc<0x50000500, 4, clock_ahb<0x20000000>>;

        using timer_02 = zoal::arch::stm32f3::general_purpose_timer<0x40000000, 2, clock_apb1<0x00000001>>;
        using timer_03 = zoal::arch::stm32f3::general_purpose_timer<0x40000400, 3, clock_apb1<0x00000002>>;
        using timer_04 = zoal::arch::stm32f3::general_purpose_timer<0x40000800, 4, clock_apb1<0x00000004>>;

        using usart_01 = typename ::zoal::arch::stm32x::usart<0x40013800, clock_apb2<0x00004000>>;

        using usart_02 = typename ::zoal::arch::stm32x::usart<0x40004400, clock_apb1<0x00020000>>;

        using usart_03 = typename ::zoal::arch::stm32x::usart<0x40004800, clock_apb1<0x00040000>>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using pa_00 = pin<port_a, 0x00>;
        using pa_01 = pin<port_a, 0x01>;
        using pa_02 = pin<port_a, 0x02>;
        using pa_03 = pin<port_a, 0x03>;
        using pa_04 = pin<port_a, 0x04>;
        using pa_05 = pin<port_a, 0x05>;
        using pa_06 = pin<port_a, 0x06>;
        using pa_07 = pin<port_a, 0x07>;
        using pa_08 = pin<port_a, 0x08>;
        using pa_09 = pin<port_a, 0x09>;
        using pa_10 = pin<port_a, 0x0A>;
        using pa_11 = pin<port_a, 0x0B>;
        using pa_12 = pin<port_a, 0x0C>;
        using pa_13 = pin<port_a, 0x0D>;
        using pa_14 = pin<port_a, 0x0E>;
        using pa_15 = pin<port_a, 0x0F>;

        using pb_00 = pin<port_b, 0x00>;
        using pb_01 = pin<port_b, 0x01>;
        using pb_02 = pin<port_b, 0x02>;
        using pb_03 = pin<port_b, 0x03>;
        using pb_04 = pin<port_b, 0x04>;
        using pb_05 = pin<port_b, 0x05>;
        using pb_06 = pin<port_b, 0x06>;
        using pb_07 = pin<port_b, 0x07>;
        using pb_08 = pin<port_b, 0x08>;
        using pb_09 = pin<port_b, 0x09>;
        using pb_10 = pin<port_b, 0x0A>;
        using pb_11 = pin<port_b, 0x0B>;
        using pb_12 = pin<port_b, 0x0C>;
        using pb_13 = pin<port_b, 0x0D>;
        using pb_14 = pin<port_b, 0x0E>;
        using pb_15 = pin<port_b, 0x0F>;

        using pc_00 = pin<port_c, 0x00>;
        using pc_01 = pin<port_c, 0x01>;
        using pc_02 = pin<port_c, 0x02>;
        using pc_03 = pin<port_c, 0x03>;
        using pc_04 = pin<port_c, 0x04>;
        using pc_05 = pin<port_c, 0x05>;
        using pc_06 = pin<port_c, 0x06>;
        using pc_07 = pin<port_c, 0x07>;
        using pc_08 = pin<port_c, 0x08>;
        using pc_09 = pin<port_c, 0x09>;
        using pc_10 = pin<port_c, 0x0A>;
        using pc_11 = pin<port_c, 0x0B>;
        using pc_12 = pin<port_c, 0x0C>;
        using pc_13 = pin<port_c, 0x0D>;
        using pc_14 = pin<port_c, 0x0E>;
        using pc_15 = pin<port_c, 0x0F>;

        using pd_02 = pin<port_d, 0x02>;

        using pf_00 = pin<port_f, 0x00>;
        using pf_01 = pin<port_f, 0x01>;

        using ports = ::zoal::ct::type_list<port_a, port_b, port_c, port_d, port_f>;
        using api = ::zoal::gpio::api<ports>;
        using mux = ::zoal::arch::stm32x::mux<self_type>;
        using cfg = ::zoal::arch::stm32x::cfg<self_type>;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

namespace zoal { namespace metadata {
    using zoal::ct::integral_constant;

    template<> // PA0 -> TIM2_CH1
    struct stm32_af<0x40000000, 0x48000000, 0x00, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA5 -> TIM2_CH1
    struct stm32_af<0x40000000, 0x48000000, 0x05, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA15 -> TIM2_CH1
    struct stm32_af<0x40000000, 0x48000000, 0x0F, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PD3 -> TIM2_CH1
    struct stm32_af<0x40000000, 0x48000C00, 0x03, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA1 -> TIM2_CH2
    struct stm32_af<0x40000000, 0x48000000, 0x01, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB3 -> TIM2_CH2
    struct stm32_af<0x40000000, 0x48000400, 0x03, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PD4 -> TIM2_CH2
    struct stm32_af<0x40000000, 0x48000C00, 0x04, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA2 -> TIM2_CH3
    struct stm32_af<0x40000000, 0x48000000, 0x02, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA9 -> TIM2_CH3
    struct stm32_af<0x40000000, 0x48000000, 0x09, signal::ch3> : zoal::ct::integral_constant<int, 10> {};
    template<> // PB10 -> TIM2_CH3
    struct stm32_af<0x40000000, 0x48000400, 0x0A, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PD7 -> TIM2_CH3
    struct stm32_af<0x40000000, 0x48000C00, 0x07, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA3 -> TIM2_CH4
    struct stm32_af<0x40000000, 0x48000000, 0x03, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA10 -> TIM2_CH4
    struct stm32_af<0x40000000, 0x48000000, 0x0A, signal::ch4> : zoal::ct::integral_constant<int, 10> {};
    template<> // PB11 -> TIM2_CH4
    struct stm32_af<0x40000000, 0x48000400, 0x0B, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PD6 -> TIM2_CH4
    struct stm32_af<0x40000000, 0x48000C00, 0x06, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA0 -> TIM2_ETR
    struct stm32_af<0x40000000, 0x48000000, 0x00, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA5 -> TIM2_ETR
    struct stm32_af<0x40000000, 0x48000000, 0x05, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA15 -> TIM2_ETR
    struct stm32_af<0x40000000, 0x48000000, 0x0F, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PD3 -> TIM2_ETR
    struct stm32_af<0x40000000, 0x48000C00, 0x03, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA6 -> TIM3_CH1
    struct stm32_af<0x40000400, 0x48000000, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB4 -> TIM3_CH1
    struct stm32_af<0x40000400, 0x48000400, 0x04, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC6 -> TIM3_CH1
    struct stm32_af<0x40000400, 0x48000800, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PE2 -> TIM3_CH1
    struct stm32_af<0x40000400, 0x48001000, 0x02, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA4 -> TIM3_CH2
    struct stm32_af<0x40000400, 0x48000000, 0x04, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA7 -> TIM3_CH2
    struct stm32_af<0x40000400, 0x48000000, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB5 -> TIM3_CH2
    struct stm32_af<0x40000400, 0x48000400, 0x05, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC7 -> TIM3_CH2
    struct stm32_af<0x40000400, 0x48000800, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PE3 -> TIM3_CH2
    struct stm32_af<0x40000400, 0x48001000, 0x03, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB0 -> TIM3_CH3
    struct stm32_af<0x40000400, 0x48000400, 0x00, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC8 -> TIM3_CH3
    struct stm32_af<0x40000400, 0x48000800, 0x08, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PE4 -> TIM3_CH3
    struct stm32_af<0x40000400, 0x48001000, 0x04, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB1 -> TIM3_CH4
    struct stm32_af<0x40000400, 0x48000400, 0x01, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB7 -> TIM3_CH4
    struct stm32_af<0x40000400, 0x48000400, 0x07, signal::ch4> : zoal::ct::integral_constant<int, 10> {};
    template<> // PC9 -> TIM3_CH4
    struct stm32_af<0x40000400, 0x48000800, 0x09, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PE5 -> TIM3_CH4
    struct stm32_af<0x40000400, 0x48001000, 0x05, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB3 -> TIM3_ETR
    struct stm32_af<0x40000400, 0x48000400, 0x03, signal::etr> : zoal::ct::integral_constant<int, 10> {};
    template<> // PD2 -> TIM3_ETR
    struct stm32_af<0x40000400, 0x48000C00, 0x02, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA11 -> TIM4_CH1
    struct stm32_af<0x40000800, 0x48000000, 0x0B, signal::ch1> : zoal::ct::integral_constant<int, 10> {};
    template<> // PB6 -> TIM4_CH1
    struct stm32_af<0x40000800, 0x48000400, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD12 -> TIM4_CH1
    struct stm32_af<0x40000800, 0x48000C00, 0x0C, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA12 -> TIM4_CH2
    struct stm32_af<0x40000800, 0x48000000, 0x0C, signal::ch2> : zoal::ct::integral_constant<int, 10> {};
    template<> // PB7 -> TIM4_CH2
    struct stm32_af<0x40000800, 0x48000400, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD13 -> TIM4_CH2
    struct stm32_af<0x40000800, 0x48000C00, 0x0D, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA13 -> TIM4_CH3
    struct stm32_af<0x40000800, 0x48000000, 0x0D, signal::ch3> : zoal::ct::integral_constant<int, 10> {};
    template<> // PB8 -> TIM4_CH3
    struct stm32_af<0x40000800, 0x48000400, 0x08, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD14 -> TIM4_CH3
    struct stm32_af<0x40000800, 0x48000C00, 0x0E, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB9 -> TIM4_CH4
    struct stm32_af<0x40000800, 0x48000400, 0x09, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD15 -> TIM4_CH4
    struct stm32_af<0x40000800, 0x48000C00, 0x0F, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PF6 -> TIM4_CH4
    struct stm32_af<0x40000800, 0x48001400, 0x06, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA8 -> TIM4_ETR
    struct stm32_af<0x40000800, 0x48000000, 0x08, signal::etr> : zoal::ct::integral_constant<int, 10> {};
    template<> // PB3 -> TIM4_ETR
    struct stm32_af<0x40000800, 0x48000400, 0x03, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PE0 -> TIM4_ETR
    struct stm32_af<0x40000800, 0x48001000, 0x00, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA8 -> USART1_CK
    struct stm32_af<0x40013800, 0x48000000, 0x08, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA11 -> USART1_CTS
    struct stm32_af<0x40013800, 0x48000000, 0x0B, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA12 -> USART1_DE
    struct stm32_af<0x40013800, 0x48000000, 0x0C, signal::de> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA12 -> USART1_RTS
    struct stm32_af<0x40013800, 0x48000000, 0x0C, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA10 -> USART1_RX
    struct stm32_af<0x40013800, 0x48000000, 0x0A, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB7 -> USART1_RX
    struct stm32_af<0x40013800, 0x48000400, 0x07, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PC5 -> USART1_RX
    struct stm32_af<0x40013800, 0x48000800, 0x05, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PE1 -> USART1_RX
    struct stm32_af<0x40013800, 0x48001000, 0x01, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA9 -> USART1_TX
    struct stm32_af<0x40013800, 0x48000000, 0x09, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB6 -> USART1_TX
    struct stm32_af<0x40013800, 0x48000400, 0x06, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PC4 -> USART1_TX
    struct stm32_af<0x40013800, 0x48000800, 0x04, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PE0 -> USART1_TX
    struct stm32_af<0x40013800, 0x48001000, 0x00, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA4 -> USART2_CK
    struct stm32_af<0x40004400, 0x48000000, 0x04, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB5 -> USART2_CK
    struct stm32_af<0x40004400, 0x48000400, 0x05, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD7 -> USART2_CK
    struct stm32_af<0x40004400, 0x48000C00, 0x07, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA0 -> USART2_CTS
    struct stm32_af<0x40004400, 0x48000000, 0x00, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD3 -> USART2_CTS
    struct stm32_af<0x40004400, 0x48000C00, 0x03, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA1 -> USART2_DE
    struct stm32_af<0x40004400, 0x48000000, 0x01, signal::de> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD4 -> USART2_DE
    struct stm32_af<0x40004400, 0x48000C00, 0x04, signal::de> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA1 -> USART2_RTS
    struct stm32_af<0x40004400, 0x48000000, 0x01, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD4 -> USART2_RTS
    struct stm32_af<0x40004400, 0x48000C00, 0x04, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA3 -> USART2_RX
    struct stm32_af<0x40004400, 0x48000000, 0x03, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA15 -> USART2_RX
    struct stm32_af<0x40004400, 0x48000000, 0x0F, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB4 -> USART2_RX
    struct stm32_af<0x40004400, 0x48000400, 0x04, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD6 -> USART2_RX
    struct stm32_af<0x40004400, 0x48000C00, 0x06, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA2 -> USART2_TX
    struct stm32_af<0x40004400, 0x48000000, 0x02, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA14 -> USART2_TX
    struct stm32_af<0x40004400, 0x48000000, 0x0E, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB3 -> USART2_TX
    struct stm32_af<0x40004400, 0x48000400, 0x03, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD5 -> USART2_TX
    struct stm32_af<0x40004400, 0x48000C00, 0x05, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB12 -> USART3_CK
    struct stm32_af<0x40004800, 0x48000400, 0x0C, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PC12 -> USART3_CK
    struct stm32_af<0x40004800, 0x48000800, 0x0C, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD10 -> USART3_CK
    struct stm32_af<0x40004800, 0x48000C00, 0x0A, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA13 -> USART3_CTS
    struct stm32_af<0x40004800, 0x48000000, 0x0D, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB13 -> USART3_CTS
    struct stm32_af<0x40004800, 0x48000400, 0x0D, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD11 -> USART3_CTS
    struct stm32_af<0x40004800, 0x48000C00, 0x0B, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB14 -> USART3_DE
    struct stm32_af<0x40004800, 0x48000400, 0x0E, signal::de> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD12 -> USART3_DE
    struct stm32_af<0x40004800, 0x48000C00, 0x0C, signal::de> : zoal::ct::integral_constant<int, 7> {};
    template<> // PF6 -> USART3_DE
    struct stm32_af<0x40004800, 0x48001400, 0x06, signal::de> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB14 -> USART3_RTS
    struct stm32_af<0x40004800, 0x48000400, 0x0E, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD12 -> USART3_RTS
    struct stm32_af<0x40004800, 0x48000C00, 0x0C, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PF6 -> USART3_RTS
    struct stm32_af<0x40004800, 0x48001400, 0x06, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB8 -> USART3_RX
    struct stm32_af<0x40004800, 0x48000400, 0x08, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB11 -> USART3_RX
    struct stm32_af<0x40004800, 0x48000400, 0x0B, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PC11 -> USART3_RX
    struct stm32_af<0x40004800, 0x48000800, 0x0B, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD9 -> USART3_RX
    struct stm32_af<0x40004800, 0x48000C00, 0x09, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PE15 -> USART3_RX
    struct stm32_af<0x40004800, 0x48001000, 0x0F, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB9 -> USART3_TX
    struct stm32_af<0x40004800, 0x48000400, 0x09, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB10 -> USART3_TX
    struct stm32_af<0x40004800, 0x48000400, 0x0A, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PC10 -> USART3_TX
    struct stm32_af<0x40004800, 0x48000800, 0x0A, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD8 -> USART3_TX
    struct stm32_af<0x40004800, 0x48000C00, 0x08, signal::tx> : zoal::ct::integral_constant<int, 7> {};
}}

#endif

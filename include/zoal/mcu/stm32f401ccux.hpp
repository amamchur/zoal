/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_STM32F401CCUX_HPP
#define ZOAL_MCU_STM32F401CCUX_HPP

#include <cstdint>
#include <zoal/arch/cortex/stm32f4/adc.hpp>
#include <zoal/arch/cortex/stm32f4/adc_common.hpp>
#include <zoal/arch/cortex/stm32f4/cfg.hpp>
#include <zoal/arch/cortex/stm32f4/i2c.hpp>
#include <zoal/arch/cortex/stm32f4/mux.hpp>
#include <zoal/arch/cortex/stm32f4/port.hpp>
#include <zoal/arch/cortex/stm32f4/rcc.hpp>
#include <zoal/arch/cortex/stm32f4/spi.hpp>
#include <zoal/arch/cortex/stm32f4/timer.hpp>
#include <zoal/arch/cortex/stm32f4/usart.hpp>
#include <zoal/arch/cortex/stm32x/bus_clock.hpp>
#include <zoal/arch/cortex/stm32x/metadata.hpp>
#include <zoal/arch/enable.hpp>
#include <zoal/arch/power.hpp>
#include <zoal/ct/signature.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/gpio/pin.hpp>

namespace zoal { namespace mcu {
    class stm32f401ccux {
    public:
        using self_type = stm32f401ccux;
        using signature = zoal::ct::signature<'s', 't', 'm', '3', '2', 'f', '4', '0', '1', 'c', 'c', 'u', 'x'>;

        using rcc = ::zoal::arch::stm32f4::rcc<0x40023800>;

        template<uint32_t Mask>
        using clock_ahb = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb, Mask>;

        template<uint32_t Mask>
        using clock_ahb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb1, Mask>;

        template<uint32_t Mask>
        using clock_ahb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb2, Mask>;

        template<uint32_t Mask>
        using clock_apb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb1, Mask>;

        template<uint32_t Mask>
        using clock_apb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb2, Mask>;

        template<uintptr_t Address, class Clock>
        using adc = typename ::zoal::arch::stm32f4::adc<Address, Clock>;

        template<uintptr_t Address, class Clock, uint32_t PinMask>
        using port = typename ::zoal::arch::stm32x::port<Address, Clock, PinMask>;

        using port_a = port<0x40020000, clock_ahb1<0x00000001>, 0xFFFF>;
        using port_b = port<0x40020400, clock_ahb1<0x00000002>, 0xF7FF>;
        using port_c = port<0x40020800, clock_ahb1<0x00000004>, 0xE000>;
        using port_h = port<0x40021C00, clock_ahb1<0x00000080>, 0x0003>;

        using adc_common = ::zoal::arch::stm32f4::adc_common<0x40012300>;

        using adc_01 = ::zoal::arch::stm32f4::adc<0x40012000, adc_common, clock_apb2<0x00000100>>;

        using i2c_01 = ::zoal::arch::stm32f4::i2c<0x40005400, clock_apb1<0x00200000>>;
        using i2c_02 = ::zoal::arch::stm32f4::i2c<0x40005800, clock_apb1<0x00400000>>;
        using i2c_03 = ::zoal::arch::stm32f4::i2c<0x40005C00, clock_apb1<0x00800000>>;

        using spi_01 = ::zoal::arch::stm32f4::spi<0x40013000, clock_apb2<0x00001000>>;
        using spi_02 = ::zoal::arch::stm32f4::spi<0x40003800, clock_apb1<0x00004000>>;
        using spi_03 = ::zoal::arch::stm32f4::spi<0x40003C00, clock_apb1<0x00008000>>;

        using timer_01 = zoal::arch::stm32f4::timer<0x40010000, clock_apb2<0x00000001>>;
        using timer_10 = zoal::arch::stm32f4::timer<0x40014400, clock_apb2<0x00020000>>;
        using timer_11 = zoal::arch::stm32f4::timer<0x40014800, clock_apb2<0x00040000>>;
        using timer_02 = zoal::arch::stm32f4::timer<0x40000000, clock_apb1<0x00000001>>;
        using timer_03 = zoal::arch::stm32f4::timer<0x40000400, clock_apb1<0x00000002>>;
        using timer_04 = zoal::arch::stm32f4::timer<0x40000800, clock_apb1<0x00000004>>;
        using timer_05 = zoal::arch::stm32f4::timer<0x40000C00, clock_apb1<0x00000008>>;
        using timer_09 = zoal::arch::stm32f4::timer<0x40014000, clock_apb2<0x00010000>>;

        using usart_01 = typename ::zoal::arch::stm32f4::usart<0x40011000, clock_apb2<0x00000010>>;
        using usart_02 = typename ::zoal::arch::stm32f4::usart<0x40004400, clock_apb1<0x00020000>>;
        using usart_06 = typename ::zoal::arch::stm32f4::usart<0x40011400, clock_apb2<0x00000020>>;

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
        using pb_12 = pin<port_b, 0x0C>;
        using pb_13 = pin<port_b, 0x0D>;
        using pb_14 = pin<port_b, 0x0E>;
        using pb_15 = pin<port_b, 0x0F>;

        using pc_13 = pin<port_c, 0x0D>;
        using pc_14 = pin<port_c, 0x0E>;
        using pc_15 = pin<port_c, 0x0F>;

        using ph_00 = pin<port_h, 0x00>;
        using ph_01 = pin<port_h, 0x01>;

        using ports = ::zoal::ct::type_list<port_a, port_b, port_c, port_h>;
        using api = ::zoal::gpio::api;
        using mux = ::zoal::arch::stm32f4::mux<self_type>;
        using cfg = ::zoal::arch::stm32f4::cfg<self_type>;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

namespace zoal { namespace metadata {
    using zoal::ct::integral_constant;
    using stm32f401ccux_sign = zoal::ct::signature<'s', 't', 'm', '3', '2', 'f', '4', '0', '1', 'c', 'c', 'u', 'x'>;

    template<> // PA0-WKUP -> TIM2_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x00, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA0-WKUP -> TIM2_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x00, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA1 -> TIM2_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x01, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA2 -> TIM2_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x02, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA3 -> TIM2_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x03, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA5 -> TIM2_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x05, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA5 -> TIM2_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x05, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA15 -> TIM2_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x0F, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA15 -> TIM2_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020000, 0x0F, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB3 -> TIM2_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020400, 0x03, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB10 -> TIM2_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020400, 0x0A, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB11 -> TIM2_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000000, 0x40020400, 0x0B, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA6 -> TIM3_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020000, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA7 -> TIM3_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020000, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB0 -> TIM3_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020400, 0x00, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB1 -> TIM3_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020400, 0x01, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB4 -> TIM3_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020400, 0x04, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB5 -> TIM3_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020400, 0x05, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC6 -> TIM3_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020800, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC7 -> TIM3_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020800, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC8 -> TIM3_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020800, 0x08, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PC9 -> TIM3_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020800, 0x09, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD2 -> TIM3_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000400, 0x40020C00, 0x02, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB6 -> TIM4_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020400, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB7 -> TIM4_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020400, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB8 -> TIM4_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020400, 0x08, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB9 -> TIM4_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020400, 0x09, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD12 -> TIM4_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020C00, 0x0C, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD13 -> TIM4_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020C00, 0x0D, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD14 -> TIM4_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020C00, 0x0E, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PD15 -> TIM4_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40020C00, 0x0F, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PE0 -> TIM4_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000800, 0x40021000, 0x00, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA0-WKUP -> TIM5_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000C00, 0x40020000, 0x00, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA1 -> TIM5_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000C00, 0x40020000, 0x01, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA2 -> TIM5_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000C00, 0x40020000, 0x02, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA3 -> TIM5_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40000C00, 0x40020000, 0x03, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB9 -> SPI2_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x09, signal::nss> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB9 -> I2S2_WS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x09, signal::ws> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB10 -> I2S2_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0A, signal::ck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB10 -> SPI2_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0A, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB12 -> SPI2_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0C, signal::nss> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB12 -> I2S2_WS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0C, signal::ws> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB13 -> I2S2_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0D, signal::ck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB13 -> SPI2_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0D, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB14 -> SPI2_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0E, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB15 -> SPI2_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0F, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB15 -> I2S2_SD
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020400, 0x0F, signal::sd> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC2 -> SPI2_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020800, 0x02, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC3 -> SPI2_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020800, 0x03, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC3 -> I2S2_SD
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020800, 0x03, signal::sd> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC6 -> I2S2_MCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020800, 0x06, signal::mck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PD3 -> I2S2_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020C00, 0x03, signal::ck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PD3 -> SPI2_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003800, 0x40020C00, 0x03, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA4 -> SPI3_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020000, 0x04, signal::nss> : zoal::ct::integral_constant<int, 6> {};
    template<> // PA4 -> I2S3_WS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020000, 0x04, signal::ws> : zoal::ct::integral_constant<int, 6> {};
    template<> // PA15 -> SPI3_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020000, 0x0F, signal::nss> : zoal::ct::integral_constant<int, 6> {};
    template<> // PA15 -> I2S3_WS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020000, 0x0F, signal::ws> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB3 -> I2S3_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x03, signal::ck> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB3 -> SPI3_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x03, signal::sck> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB4 -> SPI3_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x04, signal::miso> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB5 -> SPI3_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x05, signal::mosi> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB5 -> I2S3_SD
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x05, signal::sd> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB12 -> I2S2ext_WS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x0C, signal::ws> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB13 -> I2S2ext_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x0D, signal::ck> : zoal::ct::integral_constant<int, 6> {};
    template<> // PB15 -> I2S2ext_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020400, 0x0F, signal::miso> : zoal::ct::integral_constant<int, 6> {};
    template<> // PC7 -> I2S3_MCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020800, 0x07, signal::mck> : zoal::ct::integral_constant<int, 6> {};
    template<> // PC10 -> I2S3_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020800, 0x0A, signal::ck> : zoal::ct::integral_constant<int, 6> {};
    template<> // PC10 -> SPI3_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020800, 0x0A, signal::sck> : zoal::ct::integral_constant<int, 6> {};
    template<> // PC11 -> SPI3_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020800, 0x0B, signal::miso> : zoal::ct::integral_constant<int, 6> {};
    template<> // PC12 -> SPI3_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020800, 0x0C, signal::mosi> : zoal::ct::integral_constant<int, 6> {};
    template<> // PC12 -> I2S3_SD
    struct stm32_signal_map<stm32f401ccux_sign, 0x40003C00, 0x40020800, 0x0C, signal::sd> : zoal::ct::integral_constant<int, 6> {};
    template<> // PA0-WKUP -> USART2_CTS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020000, 0x00, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA1 -> USART2_RTS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020000, 0x01, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA2 -> USART2_TX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020000, 0x02, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA3 -> USART2_RX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020000, 0x03, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA4 -> USART2_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020000, 0x04, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD3 -> USART2_CTS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020C00, 0x03, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD4 -> USART2_RTS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020C00, 0x04, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD5 -> USART2_TX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020C00, 0x05, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD6 -> USART2_RX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020C00, 0x06, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PD7 -> USART2_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40004400, 0x40020C00, 0x07, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB5 -> I2C1_SMBA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005400, 0x40020400, 0x05, signal::smba> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB6 -> I2C1_SCL
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005400, 0x40020400, 0x06, signal::scl> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB7 -> I2C1_SDA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005400, 0x40020400, 0x07, signal::sda> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB8 -> I2C1_SCL
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005400, 0x40020400, 0x08, signal::scl> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB9 -> I2C1_SDA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005400, 0x40020400, 0x09, signal::sda> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB3 -> I2C2_SDA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005800, 0x40020400, 0x03, signal::sda> : zoal::ct::integral_constant<int, 9> {};
    template<> // PB10 -> I2C2_SCL
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005800, 0x40020400, 0x0A, signal::scl> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB11 -> I2C2_SDA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005800, 0x40020400, 0x0B, signal::sda> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB12 -> I2C2_SMBA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005800, 0x40020400, 0x0C, signal::smba> : zoal::ct::integral_constant<int, 4> {};
    template<> // PA8 -> I2C3_SCL
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005C00, 0x40020000, 0x08, signal::scl> : zoal::ct::integral_constant<int, 4> {};
    template<> // PA9 -> I2C3_SMBA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005C00, 0x40020000, 0x09, signal::smba> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB4 -> I2C3_SDA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005C00, 0x40020400, 0x04, signal::sda> : zoal::ct::integral_constant<int, 9> {};
    template<> // PC9 -> I2C3_SDA
    struct stm32_signal_map<stm32f401ccux_sign, 0x40005C00, 0x40020800, 0x09, signal::sda> : zoal::ct::integral_constant<int, 4> {};
    template<> // PA6 -> TIM1_BKIN
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x06, signal::bkin> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA7 -> TIM1_CH1N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x07, signal::ch1n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA8 -> TIM1_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x08, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA9 -> TIM1_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x09, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA10 -> TIM1_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x0A, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA11 -> TIM1_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x0B, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA12 -> TIM1_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020000, 0x0C, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB0 -> TIM1_CH2N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020400, 0x00, signal::ch2n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB1 -> TIM1_CH3N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020400, 0x01, signal::ch3n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB12 -> TIM1_BKIN
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020400, 0x0C, signal::bkin> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB13 -> TIM1_CH1N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020400, 0x0D, signal::ch1n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB14 -> TIM1_CH2N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020400, 0x0E, signal::ch2n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB15 -> TIM1_CH3N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40020400, 0x0F, signal::ch3n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE7 -> TIM1_ETR
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x07, signal::etr> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE8 -> TIM1_CH1N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x08, signal::ch1n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE9 -> TIM1_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x09, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE10 -> TIM1_CH2N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x0A, signal::ch2n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE11 -> TIM1_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x0B, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE12 -> TIM1_CH3N
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x0C, signal::ch3n> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE13 -> TIM1_CH3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x0D, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE14 -> TIM1_CH4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x0E, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PE15 -> TIM1_BKIN
    struct stm32_signal_map<stm32f401ccux_sign, 0x40010000, 0x40021000, 0x0F, signal::bkin> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA8 -> USART1_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020000, 0x08, signal::ck> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA9 -> USART1_TX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020000, 0x09, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA10 -> USART1_RX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020000, 0x0A, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA11 -> USART1_CTS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020000, 0x0B, signal::cts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA12 -> USART1_RTS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020000, 0x0C, signal::rts> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB6 -> USART1_TX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020400, 0x06, signal::tx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PB7 -> USART1_RX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011000, 0x40020400, 0x07, signal::rx> : zoal::ct::integral_constant<int, 7> {};
    template<> // PA11 -> USART6_TX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011400, 0x40020000, 0x0B, signal::tx> : zoal::ct::integral_constant<int, 8> {};
    template<> // PA12 -> USART6_RX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011400, 0x40020000, 0x0C, signal::rx> : zoal::ct::integral_constant<int, 8> {};
    template<> // PC6 -> USART6_TX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011400, 0x40020800, 0x06, signal::tx> : zoal::ct::integral_constant<int, 8> {};
    template<> // PC7 -> USART6_RX
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011400, 0x40020800, 0x07, signal::rx> : zoal::ct::integral_constant<int, 8> {};
    template<> // PC8 -> USART6_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40011400, 0x40020800, 0x08, signal::ck> : zoal::ct::integral_constant<int, 8> {};
    template<> // PA0-WKUP -> ADC1_IN0
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x00, signal::in0> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA1 -> ADC1_IN1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x01, signal::in1> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA2 -> ADC1_IN2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x02, signal::in2> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA3 -> ADC1_IN3
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x03, signal::in3> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA4 -> ADC1_IN4
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x04, signal::in4> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA5 -> ADC1_IN5
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x05, signal::in5> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA6 -> ADC1_IN6
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x06, signal::in6> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA7 -> ADC1_IN7
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x07, signal::in7> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA11 -> ADC1_EXTI11
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x0B, signal::exti11> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA15 -> ADC1_EXTI15
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020000, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
    template<> // PB0 -> ADC1_IN8
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020400, 0x00, signal::in8> : zoal::ct::integral_constant<int, 0> {};
    template<> // PB1 -> ADC1_IN9
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020400, 0x01, signal::in9> : zoal::ct::integral_constant<int, 0> {};
    template<> // PB15 -> ADC1_EXTI15
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020400, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
    template<> // PC15-OSC32_OUT -> ADC1_EXTI15
    struct stm32_signal_map<stm32f401ccux_sign, 0x40012000, 0x40020800, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA4 -> SPI1_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020000, 0x04, signal::nss> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA5 -> SPI1_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020000, 0x05, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA6 -> SPI1_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020000, 0x06, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA7 -> SPI1_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020000, 0x07, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA14 -> I2S3ext_WS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020000, 0x0E, signal::ws> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA15 -> SPI1_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020000, 0x0F, signal::nss> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB3 -> SPI1_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020400, 0x03, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB4 -> SPI1_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020400, 0x04, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB5 -> SPI1_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020400, 0x05, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC9 -> I2S_CKIN
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020800, 0x09, signal::ckin> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC10 -> I2S3ext_CK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020800, 0x0A, signal::ck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PC12 -> I2S3ext_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020800, 0x0C, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PD6 -> SPI3_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020C00, 0x06, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PD6 -> I2S3_SD
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013000, 0x40020C00, 0x06, signal::sd> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE2 -> SPI4_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x02, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE4 -> SPI4_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x04, signal::nss> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE5 -> SPI4_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x05, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE6 -> SPI4_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x06, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE11 -> SPI4_NSS
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x0B, signal::nss> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE12 -> SPI4_SCK
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x0C, signal::sck> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE13 -> SPI4_MISO
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x0D, signal::miso> : zoal::ct::integral_constant<int, 5> {};
    template<> // PE14 -> SPI4_MOSI
    struct stm32_signal_map<stm32f401ccux_sign, 0x40013400, 0x40021000, 0x0E, signal::mosi> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA2 -> TIM9_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40014000, 0x40020000, 0x02, signal::ch1> : zoal::ct::integral_constant<int, 3> {};
    template<> // PA3 -> TIM9_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40014000, 0x40020000, 0x03, signal::ch2> : zoal::ct::integral_constant<int, 3> {};
    template<> // PE5 -> TIM9_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40014000, 0x40021000, 0x05, signal::ch1> : zoal::ct::integral_constant<int, 3> {};
    template<> // PE6 -> TIM9_CH2
    struct stm32_signal_map<stm32f401ccux_sign, 0x40014000, 0x40021000, 0x06, signal::ch2> : zoal::ct::integral_constant<int, 3> {};
    template<> // PB8 -> TIM10_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40014400, 0x40020400, 0x08, signal::ch1> : zoal::ct::integral_constant<int, 3> {};
    template<> // PB9 -> TIM11_CH1
    struct stm32_signal_map<stm32f401ccux_sign, 0x40014800, 0x40020400, 0x09, signal::ch1> : zoal::ct::integral_constant<int, 3> {};
}}

#endif

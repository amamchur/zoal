/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_STM32F103C8TX_HPP
#define ZOAL_MCU_STM32F103C8TX_HPP

#include <cstdint>
#include <zoal/arch/cortex/stm32x/bus_clock.hpp>
#include <zoal/arch/cortex/stm32f1/adc_common.hpp>
#include <zoal/arch/cortex/stm32f1/afio.hpp>
#include <zoal/arch/cortex/stm32f1/cfg.hpp>
#include <zoal/arch/cortex/stm32f1/mux.hpp>
#include <zoal/arch/cortex/stm32f1/dma.hpp>
#include <zoal/arch/cortex/stm32f1/rcc.hpp>
#include <zoal/arch/cortex/stm32x/metadata.hpp>
#include <zoal/arch/enable.hpp>
#include <zoal/arch/power.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/arch/cortex/stm32f1/adc.hpp>
#include <zoal/arch/cortex/stm32f1/port.hpp>
#include <zoal/arch/cortex/stm32f1/usart.hpp>
#include <zoal/arch/cortex/stm32f1/timer.hpp>
#include <zoal/arch/cortex/stm32f1/spi.hpp>
#include <zoal/arch/cortex/stm32f1/metadata.hpp>
#include <zoal/arch/cortex/stm32f1/i2c.hpp>
#include <zoal/ct/signature.hpp>

namespace zoal { namespace mcu {
    class stm32f103c8tx {
    public:
        using self_type = stm32f103c8tx;
        using signature = zoal::ct::signature<'s','t','m','3','2','f','1','0','3','c','8','t','x'>;

using rcc = ::zoal::arch::stm32f1::rcc<0x40021000>;

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

using afio = ::zoal::arch::stm32f1::afio<0x40010000, clock_apb2<0x00000001>>;

template<uintptr_t Address, class Clock, uint32_t PinMask>
using port = typename ::zoal::arch::stm32f1::port<Address, Clock, PinMask>;


using port_a = port<0x40010800, clock_apb2<0x00000004>, 0xFFFF>;
using port_b = port<0x40010C00, clock_apb2<0x00000008>, 0xFFFF>;
using port_c = port<0x40011000, clock_apb2<0x00000010>, 0xE000>;
using port_d = port<0x40011400, clock_apb2<0x00000020>, 0x0003>;

using adc_common = ::zoal::arch::stm32f1::adc_common<0x00000000>;


using adc_01 = ::zoal::arch::stm32f1::adc<0x40012400, adc_common, clock_apb2<0x00000200>>;
using adc_02 = ::zoal::arch::stm32f1::adc<0x40012800, adc_common, clock_apb2<0x00000400>>;
using adc_03 = ::zoal::arch::stm32f1::adc<0x40013C00, adc_common, clock_apb2<0x00008000>>;

using dma_01 = ::zoal::arch::stm32f1::dma<0x40020000, clock_ahb<0x00000001>>;
using dma_02 = ::zoal::arch::stm32f1::dma<0x40020400, clock_ahb<0x00000002>>;

using i2c_01 = ::zoal::arch::stm32f1::i2c<0x40005400, clock_apb1<0x00200000>>;
using i2c_02 = ::zoal::arch::stm32f1::i2c<0x40005800, clock_apb1<0x00400000>>;


using spi_01 = ::zoal::arch::stm32f1::spi<0x40013000, clock_apb2<0x00001000>>;
using spi_02 = ::zoal::arch::stm32f1::spi<0x40003800, clock_apb1<0x00004000>>;
using spi_03 = ::zoal::arch::stm32f1::spi<0x40003C00, clock_apb1<0x00008000>>;

using timer_01 = zoal::arch::stm32f1::timer<0x40012C00, clock_apb2<0x00000800>>;
using timer_10 = zoal::arch::stm32f1::timer<0x40015000, clock_apb2<0x00100000>>;
using timer_11 = zoal::arch::stm32f1::timer<0x40015400, clock_apb2<0x00200000>>;
using timer_12 = zoal::arch::stm32f1::timer<0x40001800, clock_apb1<0x00000040>>;
using timer_13 = zoal::arch::stm32f1::timer<0x40001C00, clock_apb1<0x00000080>>;
using timer_14 = zoal::arch::stm32f1::timer<0x40002000, clock_apb1<0x00000100>>;
using timer_02 = zoal::arch::stm32f1::timer<0x40000000, clock_apb1<0x00000001>>;
using timer_03 = zoal::arch::stm32f1::timer<0x40000400, clock_apb1<0x00000002>>;
using timer_04 = zoal::arch::stm32f1::timer<0x40000800, clock_apb1<0x00000004>>;
using timer_05 = zoal::arch::stm32f1::timer<0x40000C00, clock_apb1<0x00000008>>;
using timer_06 = zoal::arch::stm32f1::timer<0x40001000, clock_apb1<0x00000010>>;
using timer_07 = zoal::arch::stm32f1::timer<0x40001400, clock_apb1<0x00000020>>;
using timer_08 = zoal::arch::stm32f1::timer<0x40013400, clock_apb2<0x00002000>>;
using timer_09 = zoal::arch::stm32f1::timer<0x40014C00, clock_apb2<0x00080000>>;

using uart_04 = typename ::zoal::arch::stm32f1::usart<0x40004C00, clock_apb1<0x00080000>>;
using uart_05 = typename ::zoal::arch::stm32f1::usart<0x40005000, clock_apb1<0x00100000>>;

using usart_01 = typename ::zoal::arch::stm32f1::usart<0x40013800, clock_apb2<0x00004000>>;
using usart_02 = typename ::zoal::arch::stm32f1::usart<0x40004400, clock_apb1<0x00020000>>;
using usart_03 = typename ::zoal::arch::stm32f1::usart<0x40004800, clock_apb1<0x00040000>>;

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

using pc_13 = pin<port_c, 0x0D>;
using pc_14 = pin<port_c, 0x0E>;
using pc_15 = pin<port_c, 0x0F>;

using pd_00 = pin<port_d, 0x00>;
using pd_01 = pin<port_d, 0x01>;


using ports = ::zoal::ct::type_list<
port_a,
port_b,
port_c,
port_d
>;
    using api = ::zoal::gpio::api;
    using mux = ::zoal::arch::stm32f1::mux<self_type>;
    using cfg = ::zoal::arch::stm32f1::cfg<self_type>;

    template<class ... Module>
    using power = ::zoal::arch::power<Module...>;

    template<class ... Module>
    using enable = ::zoal::arch::enable<Module...>;
    };
}}

namespace zoal { namespace metadata {
    using zoal::ct::integral_constant;
    using stm32f103c8tx_sign = zoal::ct::signature<'s','t','m','3','2','f','1','0','3','c','8','t','x'>;

template<> // PA0-WKUP -> TIM2_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x00, signal::ch1> : zoal::ct::integral_constant<int, 0> {};
template<> // PA0-WKUP -> TIM2_ETR
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x00, signal::etr> : zoal::ct::integral_constant<int, 0> {};
template<> // PA1 -> TIM2_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x01, signal::ch2> : zoal::ct::integral_constant<int, 0> {};
template<> // PA2 -> TIM2_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x02, signal::ch3> : zoal::ct::integral_constant<int, 0> {};
template<> // PA3 -> TIM2_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x03, signal::ch4> : zoal::ct::integral_constant<int, 0> {};
template<> // PA15 -> TIM2_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x0F, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
template<> // PA15 -> TIM2_ETR
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010800, 0x0F, signal::etr> : zoal::ct::integral_constant<int, 1> {};
template<> // PB3 -> TIM2_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010C00, 0x03, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
template<> // PB10 -> TIM2_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010C00, 0x0A, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
template<> // PB11 -> TIM2_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000000, 0x40010C00, 0x0B, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
template<> // PA6 -> TIM3_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40010800, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 0> {};
template<> // PA7 -> TIM3_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40010800, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 0> {};
template<> // PB0 -> TIM3_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40010C00, 0x00, signal::ch3> : zoal::ct::integral_constant<int, 0> {};
template<> // PB1 -> TIM3_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40010C00, 0x01, signal::ch4> : zoal::ct::integral_constant<int, 0> {};
template<> // PB4 -> TIM3_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40010C00, 0x04, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
template<> // PB5 -> TIM3_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40010C00, 0x05, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
template<> // PC6 -> TIM3_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40011000, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
template<> // PC7 -> TIM3_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40011000, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
template<> // PC8 -> TIM3_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40011000, 0x08, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
template<> // PC9 -> TIM3_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000400, 0x40011000, 0x09, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
template<> // PB6 -> TIM4_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40010C00, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 0> {};
template<> // PB7 -> TIM4_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40010C00, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 0> {};
template<> // PB8 -> TIM4_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40010C00, 0x08, signal::ch3> : zoal::ct::integral_constant<int, 0> {};
template<> // PB9 -> TIM4_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40010C00, 0x09, signal::ch4> : zoal::ct::integral_constant<int, 0> {};
template<> // PD12 -> TIM4_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40011400, 0x0C, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
template<> // PD13 -> TIM4_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40011400, 0x0D, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
template<> // PD14 -> TIM4_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40011400, 0x0E, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
template<> // PD15 -> TIM4_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40000800, 0x40011400, 0x0F, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
template<> // PB12 -> SPI2_NSS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40003800, 0x40010C00, 0x0C, signal::nss> : zoal::ct::integral_constant<int, 0> {};
template<> // PB13 -> SPI2_SCK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40003800, 0x40010C00, 0x0D, signal::sck> : zoal::ct::integral_constant<int, 0> {};
template<> // PB14 -> SPI2_MISO
struct stm32_signal_map<stm32f103c8tx_sign, 0x40003800, 0x40010C00, 0x0E, signal::miso> : zoal::ct::integral_constant<int, 0> {};
template<> // PB15 -> SPI2_MOSI
struct stm32_signal_map<stm32f103c8tx_sign, 0x40003800, 0x40010C00, 0x0F, signal::mosi> : zoal::ct::integral_constant<int, 0> {};
template<> // PA0-WKUP -> USART2_CTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40010800, 0x00, signal::cts> : zoal::ct::integral_constant<int, 0> {};
template<> // PA1 -> USART2_RTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40010800, 0x01, signal::rts> : zoal::ct::integral_constant<int, 0> {};
template<> // PA2 -> USART2_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40010800, 0x02, signal::tx> : zoal::ct::integral_constant<int, 0> {};
template<> // PA3 -> USART2_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40010800, 0x03, signal::rx> : zoal::ct::integral_constant<int, 0> {};
template<> // PA4 -> USART2_CK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40010800, 0x04, signal::ck> : zoal::ct::integral_constant<int, 0> {};
template<> // PD3 -> USART2_CTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40011400, 0x03, signal::cts> : zoal::ct::integral_constant<int, 1> {};
template<> // PD4 -> USART2_RTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40011400, 0x04, signal::rts> : zoal::ct::integral_constant<int, 1> {};
template<> // PD5 -> USART2_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40011400, 0x05, signal::tx> : zoal::ct::integral_constant<int, 1> {};
template<> // PD6 -> USART2_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40011400, 0x06, signal::rx> : zoal::ct::integral_constant<int, 1> {};
template<> // PD7 -> USART2_CK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004400, 0x40011400, 0x07, signal::ck> : zoal::ct::integral_constant<int, 1> {};
template<> // PB10 -> USART3_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40010C00, 0x0A, signal::tx> : zoal::ct::integral_constant<int, 0> {};
template<> // PB11 -> USART3_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40010C00, 0x0B, signal::rx> : zoal::ct::integral_constant<int, 0> {};
template<> // PB12 -> USART3_CK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40010C00, 0x0C, signal::ck> : zoal::ct::integral_constant<int, 0> {};
template<> // PB13 -> USART3_CTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40010C00, 0x0D, signal::cts> : zoal::ct::integral_constant<int, 0> {};
template<> // PB14 -> USART3_RTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40010C00, 0x0E, signal::rts> : zoal::ct::integral_constant<int, 0> {};
template<> // PC10 -> USART3_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011000, 0x0A, signal::tx> : zoal::ct::integral_constant<int, 1> {};
template<> // PC11 -> USART3_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011000, 0x0B, signal::rx> : zoal::ct::integral_constant<int, 1> {};
template<> // PC12 -> USART3_CK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011000, 0x0C, signal::ck> : zoal::ct::integral_constant<int, 1> {};
template<> // PD8 -> USART3_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011400, 0x08, signal::tx> : zoal::ct::integral_constant<int, 2> {};
template<> // PD9 -> USART3_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011400, 0x09, signal::rx> : zoal::ct::integral_constant<int, 2> {};
template<> // PD10 -> USART3_CK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011400, 0x0A, signal::ck> : zoal::ct::integral_constant<int, 2> {};
template<> // PD11 -> USART3_CTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011400, 0x0B, signal::cts> : zoal::ct::integral_constant<int, 2> {};
template<> // PD12 -> USART3_RTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40004800, 0x40011400, 0x0C, signal::rts> : zoal::ct::integral_constant<int, 2> {};
template<> // PB5 -> I2C1_SMBA
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005400, 0x40010C00, 0x05, signal::smba> : zoal::ct::integral_constant<int, 0> {};
template<> // PB6 -> I2C1_SCL
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005400, 0x40010C00, 0x06, signal::scl> : zoal::ct::integral_constant<int, 0> {};
template<> // PB7 -> I2C1_SDA
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005400, 0x40010C00, 0x07, signal::sda> : zoal::ct::integral_constant<int, 0> {};
template<> // PB8 -> I2C1_SCL
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005400, 0x40010C00, 0x08, signal::scl> : zoal::ct::integral_constant<int, 1> {};
template<> // PB9 -> I2C1_SDA
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005400, 0x40010C00, 0x09, signal::sda> : zoal::ct::integral_constant<int, 1> {};
template<> // PB10 -> I2C2_SCL
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005800, 0x40010C00, 0x0A, signal::scl> : zoal::ct::integral_constant<int, 0> {};
template<> // PB11 -> I2C2_SDA
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005800, 0x40010C00, 0x0B, signal::sda> : zoal::ct::integral_constant<int, 0> {};
template<> // PB12 -> I2C2_SMBA
struct stm32_signal_map<stm32f103c8tx_sign, 0x40005800, 0x40010C00, 0x0C, signal::smba> : zoal::ct::integral_constant<int, 0> {};
template<> // PA0-WKUP -> ADC1_IN0
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x00, signal::in0> : zoal::ct::integral_constant<int, 0> {};
template<> // PA1 -> ADC1_IN1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x01, signal::in1> : zoal::ct::integral_constant<int, 0> {};
template<> // PA2 -> ADC1_IN2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x02, signal::in2> : zoal::ct::integral_constant<int, 0> {};
template<> // PA3 -> ADC1_IN3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x03, signal::in3> : zoal::ct::integral_constant<int, 0> {};
template<> // PA4 -> ADC1_IN4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x04, signal::in4> : zoal::ct::integral_constant<int, 0> {};
template<> // PA5 -> ADC1_IN5
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x05, signal::in5> : zoal::ct::integral_constant<int, 0> {};
template<> // PA6 -> ADC1_IN6
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x06, signal::in6> : zoal::ct::integral_constant<int, 0> {};
template<> // PA7 -> ADC1_IN7
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x07, signal::in7> : zoal::ct::integral_constant<int, 0> {};
template<> // PA11 -> ADC1_EXTI11
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x0B, signal::exti11> : zoal::ct::integral_constant<int, 0> {};
template<> // PA15 -> ADC1_EXTI15
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010800, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
template<> // PB0 -> ADC1_IN8
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010C00, 0x00, signal::in8> : zoal::ct::integral_constant<int, 0> {};
template<> // PB1 -> ADC1_IN9
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010C00, 0x01, signal::in9> : zoal::ct::integral_constant<int, 0> {};
template<> // PB11 -> ADC1_EXTI11
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010C00, 0x0B, signal::exti11> : zoal::ct::integral_constant<int, 0> {};
template<> // PB15 -> ADC1_EXTI15
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40010C00, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
template<> // PC15-OSC32_OUT -> ADC1_EXTI15
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012400, 0x40011000, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
template<> // PA0-WKUP -> ADC2_IN0
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x00, signal::in0> : zoal::ct::integral_constant<int, 0> {};
template<> // PA1 -> ADC2_IN1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x01, signal::in1> : zoal::ct::integral_constant<int, 0> {};
template<> // PA2 -> ADC2_IN2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x02, signal::in2> : zoal::ct::integral_constant<int, 0> {};
template<> // PA3 -> ADC2_IN3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x03, signal::in3> : zoal::ct::integral_constant<int, 0> {};
template<> // PA4 -> ADC2_IN4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x04, signal::in4> : zoal::ct::integral_constant<int, 0> {};
template<> // PA5 -> ADC2_IN5
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x05, signal::in5> : zoal::ct::integral_constant<int, 0> {};
template<> // PA6 -> ADC2_IN6
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x06, signal::in6> : zoal::ct::integral_constant<int, 0> {};
template<> // PA7 -> ADC2_IN7
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x07, signal::in7> : zoal::ct::integral_constant<int, 0> {};
template<> // PA11 -> ADC2_EXTI11
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x0B, signal::exti11> : zoal::ct::integral_constant<int, 0> {};
template<> // PA15 -> ADC2_EXTI15
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010800, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
template<> // PB0 -> ADC2_IN8
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010C00, 0x00, signal::in8> : zoal::ct::integral_constant<int, 0> {};
template<> // PB1 -> ADC2_IN9
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010C00, 0x01, signal::in9> : zoal::ct::integral_constant<int, 0> {};
template<> // PB11 -> ADC2_EXTI11
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010C00, 0x0B, signal::exti11> : zoal::ct::integral_constant<int, 0> {};
template<> // PB15 -> ADC2_EXTI15
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40010C00, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
template<> // PC15-OSC32_OUT -> ADC2_EXTI15
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012800, 0x40011000, 0x0F, signal::exti15> : zoal::ct::integral_constant<int, 0> {};
template<> // PA6 -> TIM1_BKIN
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x06, signal::bkin> : zoal::ct::integral_constant<int, 1> {};
template<> // PA7 -> TIM1_CH1N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x07, signal::ch1n> : zoal::ct::integral_constant<int, 1> {};
template<> // PA8 -> TIM1_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x08, signal::ch1> : zoal::ct::integral_constant<int, 0> {};
template<> // PA9 -> TIM1_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x09, signal::ch2> : zoal::ct::integral_constant<int, 0> {};
template<> // PA10 -> TIM1_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x0A, signal::ch3> : zoal::ct::integral_constant<int, 0> {};
template<> // PA11 -> TIM1_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x0B, signal::ch4> : zoal::ct::integral_constant<int, 0> {};
template<> // PA12 -> TIM1_ETR
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010800, 0x0C, signal::etr> : zoal::ct::integral_constant<int, 0> {};
template<> // PB0 -> TIM1_CH2N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010C00, 0x00, signal::ch2n> : zoal::ct::integral_constant<int, 1> {};
template<> // PB1 -> TIM1_CH3N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010C00, 0x01, signal::ch3n> : zoal::ct::integral_constant<int, 1> {};
template<> // PB12 -> TIM1_BKIN
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010C00, 0x0C, signal::bkin> : zoal::ct::integral_constant<int, 0> {};
template<> // PB13 -> TIM1_CH1N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010C00, 0x0D, signal::ch1n> : zoal::ct::integral_constant<int, 0> {};
template<> // PB14 -> TIM1_CH2N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010C00, 0x0E, signal::ch2n> : zoal::ct::integral_constant<int, 0> {};
template<> // PB15 -> TIM1_CH3N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40010C00, 0x0F, signal::ch3n> : zoal::ct::integral_constant<int, 0> {};
template<> // PE7 -> TIM1_ETR
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x07, signal::etr> : zoal::ct::integral_constant<int, 2> {};
template<> // PE8 -> TIM1_CH1N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x08, signal::ch1n> : zoal::ct::integral_constant<int, 2> {};
template<> // PE9 -> TIM1_CH1
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x09, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
template<> // PE10 -> TIM1_CH2N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x0A, signal::ch2n> : zoal::ct::integral_constant<int, 2> {};
template<> // PE11 -> TIM1_CH2
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x0B, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
template<> // PE12 -> TIM1_CH3N
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x0C, signal::ch3n> : zoal::ct::integral_constant<int, 2> {};
template<> // PE13 -> TIM1_CH3
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x0D, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
template<> // PE14 -> TIM1_CH4
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x0E, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
template<> // PE15 -> TIM1_BKIN
struct stm32_signal_map<stm32f103c8tx_sign, 0x40012C00, 0x40011800, 0x0F, signal::bkin> : zoal::ct::integral_constant<int, 2> {};
template<> // PA4 -> SPI1_NSS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010800, 0x04, signal::nss> : zoal::ct::integral_constant<int, 0> {};
template<> // PA5 -> SPI1_SCK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010800, 0x05, signal::sck> : zoal::ct::integral_constant<int, 0> {};
template<> // PA6 -> SPI1_MISO
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010800, 0x06, signal::miso> : zoal::ct::integral_constant<int, 0> {};
template<> // PA7 -> SPI1_MOSI
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010800, 0x07, signal::mosi> : zoal::ct::integral_constant<int, 0> {};
template<> // PA15 -> SPI1_NSS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010800, 0x0F, signal::nss> : zoal::ct::integral_constant<int, 1> {};
template<> // PB3 -> SPI1_SCK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010C00, 0x03, signal::sck> : zoal::ct::integral_constant<int, 1> {};
template<> // PB4 -> SPI1_MISO
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010C00, 0x04, signal::miso> : zoal::ct::integral_constant<int, 1> {};
template<> // PB5 -> SPI1_MOSI
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013000, 0x40010C00, 0x05, signal::mosi> : zoal::ct::integral_constant<int, 1> {};
template<> // PA8 -> USART1_CK
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010800, 0x08, signal::ck> : zoal::ct::integral_constant<int, 0> {};
template<> // PA9 -> USART1_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010800, 0x09, signal::tx> : zoal::ct::integral_constant<int, 0> {};
template<> // PA10 -> USART1_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010800, 0x0A, signal::rx> : zoal::ct::integral_constant<int, 0> {};
template<> // PA11 -> USART1_CTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010800, 0x0B, signal::cts> : zoal::ct::integral_constant<int, 0> {};
template<> // PA12 -> USART1_RTS
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010800, 0x0C, signal::rts> : zoal::ct::integral_constant<int, 0> {};
template<> // PB6 -> USART1_TX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010C00, 0x06, signal::tx> : zoal::ct::integral_constant<int, 1> {};
template<> // PB7 -> USART1_RX
struct stm32_signal_map<stm32f103c8tx_sign, 0x40013800, 0x40010C00, 0x07, signal::rx> : zoal::ct::integral_constant<int, 1> {};
}}

#endif

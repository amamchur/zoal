/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_STM32F030F4PX_HPP
#define ZOAL_MCU_STM32F030F4PX_HPP

#include <cstdint>
#include <zoal/arch/cortex/stm32x/bus_clock.hpp>
#include <zoal/arch/cortex/stm32x/cfg.hpp>
#include <zoal/arch/cortex/stm32x/i2c.hpp>
#include <zoal/arch/cortex/stm32x/metadata.hpp>
#include <zoal/arch/cortex/stm32x/mux.hpp>
#include <zoal/arch/cortex/stm32x/port.hpp>
#include <zoal/arch/cortex/stm32x/rcc.hpp>
#include <zoal/arch/cortex/stm32x/timer.hpp>
#include <zoal/arch/cortex/stm32x/usart.hpp>
#include <zoal/arch/enable.hpp>
#include <zoal/arch/power.hpp>
#include <zoal/ct/signature.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/gpio/pin.hpp>

namespace zoal { namespace mcu {
    class stm32f030f4px {
    public:
        using self_type = stm32f030f4px;
        using signature = zoal::ct::signature<'s', 't', 'm', '3', '2', 'f', '0', '3', '0', 'f', '4', 'p', 'x'>;

        using rcc = ::zoal::arch::stm32x::rcc<>;

        template<uint32_t Mask>
        using clock_ahb = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb, Mask>;

        template<uint32_t Mask>
        using clock_apb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb1, Mask>;

        template<uint32_t Mask>
        using clock_apb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb2, Mask>;

        template<uintptr_t Address, class Clock, uint32_t PinMask>
        using port = typename ::zoal::arch::stm32x::port<Address, Clock, PinMask>;

        using port_a = port<0x48000000, clock_ahb<0x00020000>, 0x66FF>;
        using port_b = port<0x48000400, clock_ahb<0x00040000>, 0x0002>;
        using port_f = port<0x48001400, clock_ahb<0x00400000>, 0x0003>;

        using i2c_01 = ::zoal::arch::stm32x::i2c<0x40005400, clock_apb1<0x00200000>>;

        using timer_01 = zoal::arch::stm32x::timer<0x40012C00, clock_apb2<0x00000800>>;
        using timer_14 = zoal::arch::stm32x::timer<0x40002000, clock_apb1<0x00000100>>;
        using timer_16 = zoal::arch::stm32x::timer<0x40014400, clock_apb2<0x00020000>>;
        using timer_17 = zoal::arch::stm32x::timer<0x40014800, clock_apb2<0x00040000>>;
        using timer_03 = zoal::arch::stm32x::timer<0x40000400, clock_apb1<0x00000002>>;

        using usart_01 = typename ::zoal::arch::stm32x::usart<0x40013800, clock_apb2<0x00004000>>;

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
        using pa_09 = pin<port_a, 0x09>;
        using pa_10 = pin<port_a, 0x0A>;
        using pa_13 = pin<port_a, 0x0D>;
        using pa_14 = pin<port_a, 0x0E>;

        using pb_01 = pin<port_b, 0x01>;

        using pf_00 = pin<port_f, 0x00>;
        using pf_01 = pin<port_f, 0x01>;

        using ports = ::zoal::ct::type_list<port_a, port_b, port_f>;
        using api = ::zoal::gpio::api;
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
    using stm32f030f4px_sign = zoal::ct::signature<'s', 't', 'm', '3', '2', 'f', '0', '3', '0', 'f', '4', 'p', 'x'>;

    template<> // PA6 -> TIM3_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40000400, 0x48000000, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA7 -> TIM3_CH2
    struct stm32_signal_map<stm32f030f4px_sign, 0x40000400, 0x48000000, 0x07, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB0 -> TIM3_CH3
    struct stm32_signal_map<stm32f030f4px_sign, 0x40000400, 0x48000400, 0x00, signal::ch3> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB1 -> TIM3_CH4
    struct stm32_signal_map<stm32f030f4px_sign, 0x40000400, 0x48000400, 0x01, signal::ch4> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB4 -> TIM3_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40000400, 0x48000400, 0x04, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB5 -> TIM3_CH2
    struct stm32_signal_map<stm32f030f4px_sign, 0x40000400, 0x48000400, 0x05, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA4 -> TIM14_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40002000, 0x48000000, 0x04, signal::ch1> : zoal::ct::integral_constant<int, 4> {};
    template<> // PA7 -> TIM14_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40002000, 0x48000000, 0x07, signal::ch1> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB1 -> TIM14_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40002000, 0x48000400, 0x01, signal::ch1> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA0 -> USART2_CTS
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x00, signal::cts> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA1 -> USART2_DE
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x01, signal::de> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA1 -> USART2_RTS
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x01, signal::rts> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA2 -> USART2_TX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x02, signal::tx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA3 -> USART2_RX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x03, signal::rx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA4 -> USART2_CK
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x04, signal::ck> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA14 -> USART2_TX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x0E, signal::tx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA15 -> USART2_RX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40004400, 0x48000000, 0x0F, signal::rx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA9 -> I2C1_SCL
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000000, 0x09, signal::scl> : zoal::ct::integral_constant<int, 4> {};
    template<> // PA10 -> I2C1_SDA
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000000, 0x0A, signal::sda> : zoal::ct::integral_constant<int, 4> {};
    template<> // PB5 -> I2C1_SMBA
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x05, signal::smba> : zoal::ct::integral_constant<int, 3> {};
    template<> // PB6 -> I2C1_SCL
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x06, signal::scl> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB7 -> I2C1_SDA
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x07, signal::sda> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB8 -> I2C1_SCL
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x08, signal::scl> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB9 -> I2C1_SDA
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x09, signal::sda> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB10 -> I2C1_SCL
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x0A, signal::scl> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB11 -> I2C1_SDA
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005400, 0x48000400, 0x0B, signal::sda> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB10 -> I2C2_SCL
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005800, 0x48000400, 0x0A, signal::scl> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB11 -> I2C2_SDA
    struct stm32_signal_map<stm32f030f4px_sign, 0x40005800, 0x48000400, 0x0B, signal::sda> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA6 -> TIM1_BKIN
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x06, signal::bkin> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA7 -> TIM1_CH1N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x07, signal::ch1n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA8 -> TIM1_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x08, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA9 -> TIM1_CH2
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x09, signal::ch2> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA10 -> TIM1_CH3
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x0A, signal::ch3> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA11 -> TIM1_CH4
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x0B, signal::ch4> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA12 -> TIM1_ETR
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000000, 0x0C, signal::etr> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB0 -> TIM1_CH2N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000400, 0x00, signal::ch2n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB1 -> TIM1_CH3N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000400, 0x01, signal::ch3n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB12 -> TIM1_BKIN
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000400, 0x0C, signal::bkin> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB13 -> TIM1_CH1N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000400, 0x0D, signal::ch1n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB14 -> TIM1_CH2N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000400, 0x0E, signal::ch2n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB15 -> TIM1_CH3N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40012C00, 0x48000400, 0x0F, signal::ch3n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA0 -> USART1_CTS
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x00, signal::cts> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA1 -> USART1_DE
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x01, signal::de> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA1 -> USART1_RTS
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x01, signal::rts> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA2 -> USART1_TX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x02, signal::tx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA3 -> USART1_RX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x03, signal::rx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA4 -> USART1_CK
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x04, signal::ck> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA8 -> USART1_CK
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x08, signal::ck> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA9 -> USART1_TX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x09, signal::tx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA10 -> USART1_RX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x0A, signal::rx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA11 -> USART1_CTS
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x0B, signal::cts> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA12 -> USART1_DE
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x0C, signal::de> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA12 -> USART1_RTS
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x0C, signal::rts> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA14 -> USART1_TX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x0E, signal::tx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA15 -> USART1_RX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000000, 0x0F, signal::rx> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB6 -> USART1_TX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000400, 0x06, signal::tx> : zoal::ct::integral_constant<int, 0> {};
    template<> // PB7 -> USART1_RX
    struct stm32_signal_map<stm32f030f4px_sign, 0x40013800, 0x48000400, 0x07, signal::rx> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA2 -> TIM15_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014000, 0x48000000, 0x02, signal::ch1> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA3 -> TIM15_CH2
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014000, 0x48000000, 0x03, signal::ch2> : zoal::ct::integral_constant<int, 0> {};
    template<> // PA9 -> TIM15_BKIN
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014000, 0x48000000, 0x09, signal::bkin> : zoal::ct::integral_constant<int, 0> {};
    template<> // PB14 -> TIM15_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014000, 0x48000400, 0x0E, signal::ch1> : zoal::ct::integral_constant<int, 1> {};
    template<> // PB15 -> TIM15_CH1N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014000, 0x48000400, 0x0F, signal::ch1n> : zoal::ct::integral_constant<int, 3> {};
    template<> // PB15 -> TIM15_CH2
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014000, 0x48000400, 0x0F, signal::ch2> : zoal::ct::integral_constant<int, 1> {};
    template<> // PA6 -> TIM16_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014400, 0x48000000, 0x06, signal::ch1> : zoal::ct::integral_constant<int, 5> {};
    template<> // PB5 -> TIM16_BKIN
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014400, 0x48000400, 0x05, signal::bkin> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB6 -> TIM16_CH1N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014400, 0x48000400, 0x06, signal::ch1n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB8 -> TIM16_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014400, 0x48000400, 0x08, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
    template<> // PA7 -> TIM17_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014800, 0x48000000, 0x07, signal::ch1> : zoal::ct::integral_constant<int, 5> {};
    template<> // PA10 -> TIM17_BKIN
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014800, 0x48000000, 0x0A, signal::bkin> : zoal::ct::integral_constant<int, 0> {};
    template<> // PB7 -> TIM17_CH1N
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014800, 0x48000400, 0x07, signal::ch1n> : zoal::ct::integral_constant<int, 2> {};
    template<> // PB9 -> TIM17_CH1
    struct stm32_signal_map<stm32f030f4px_sign, 0x40014800, 0x48000400, 0x09, signal::ch1> : zoal::ct::integral_constant<int, 2> {};
}}

#endif

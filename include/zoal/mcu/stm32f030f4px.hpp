/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_STM32F030F4PX_HPP
#define ZOAL_MCU_STM32F030F4PX_HPP

#include <stdint.h>
#include <zoal/arch/cortex/stm32x/bus_clock.hpp>
#include <zoal/arch/cortex/stm32x/cfg.hpp>
#include <zoal/arch/cortex/stm32x/metadata.hpp>
#include <zoal/arch/cortex/stm32x/mux.hpp>
#include <zoal/arch/cortex/stm32x/port.hpp>
#include <zoal/arch/cortex/stm32x/rcc.hpp>
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
}}

#endif

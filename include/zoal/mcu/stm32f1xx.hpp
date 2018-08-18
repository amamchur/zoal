#pragma once

#ifndef ZOAL_MCU_SMT32F0XX_HPP
#define ZOAL_MCU_SMT32F0XX_HPP

#include "base_mcu.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../arch/cortex/stm32f1/port.hpp"
#include "../arch/cortex/stm32x/reset_and_clock_controller.hpp"
#include "../arch/cortex/stm32f1/nested_vectored_interrupt_controller.hpp"
#include "../arch/cortex/stm32f1/adc.hpp"
#include "../arch/cortex/stm32f1/usart.hpp"
#include "../arch/cortex/stm32x/peripheral_clock.hpp"
#include "../arch/cortex/stm32x/interrupt_control.hpp"
#include "../utils/ms_counter.hpp"
#include "../utils/tool_set.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class stm32f1xx : public base_mcu<Frequency, 4> {
    private:
        using rcc = typename ::zoal::arch::stm32x::reset_and_clock_controller<0x40021000>;
        using nvic = typename ::zoal::arch::stm32f1::nested_vectored_interrupt_controller<0xE000E100>;

        template<uint8_t Index, uint32_t Mask>
        using irq_ctrl = ::zoal::utils::interrupt_control<nvic, Index, Mask>;

        template<uint32_t MaskAPB1ENR, uint32_t MaskAPB2ENR>
        using clock = ::zoal::stm32x::peripheral_clock<rcc, 0, MaskAPB1ENR, MaskAPB2ENR>;
    public:
        template<uintptr_t Address, class Clock>
        using port = typename ::zoal::arch::stm32f1::port<Address, Clock>;

        template<uintptr_t BaseAddress, uint8_t N, class Clock>
        using adc = typename ::zoal::arch::stm32f1::adc<BaseAddress, N, Clock>;

        template<class PIOCtrl, uint8_t b>
        using pin = typename ::zoal::gpio::pin<PIOCtrl, b>;

        using port_a = port<0x40010800, clock<0, 0x004>>;
        using port_b = port<0x40010c00, clock<0, 0x008>>;
        using port_c = port<0x40011000, clock<0, 0x010>>;
        using port_d = port<0x40011400, clock<0, 0x020>>;
        using port_e = port<0x40011800, clock<0, 0x040>>;
        using port_f = port<0x40011c00, clock<0, 0x080>>;
        using port_g = port<0x40012000, clock<0, 0x100>>;

        using adc1 = adc<0x40012400, 1, clock<0, 0x0200>>;
        using adc2 = adc<0x40012400, 2, clock<0, 0x0400>>;
        using adc3 = adc<0x40013c00, 3, clock<0, 0x8000>>;

        using pa00 = pin<port_a, 0x0>;
        using pa01 = pin<port_a, 0x1>;
        using pa02 = pin<port_a, 0x2>;
        using pa03 = pin<port_a, 0x3>;
        using pa04 = pin<port_a, 0x4>;
        using pa05 = pin<port_a, 0x5>;
        using pa06 = pin<port_a, 0x6>;
        using pa07 = pin<port_a, 0x7>;
        using pa08 = pin<port_a, 0x8>;
        using pa09 = pin<port_a, 0x9>;
        using pa10 = pin<port_a, 0xA>;
        using pa11 = pin<port_a, 0xB>;
        using pa12 = pin<port_a, 0xC>;
        using pa13 = pin<port_a, 0xD>;
        using pa14 = pin<port_a, 0xE>;
        using pa15 = pin<port_a, 0xF>;

        using pb00 = pin<port_b, 0x0>;
        using pb01 = pin<port_b, 0x1>;
        using pb02 = pin<port_b, 0x2>;
        using pb03 = pin<port_b, 0x3>;
        using pb04 = pin<port_b, 0x4>;
        using pb05 = pin<port_b, 0x5>;
        using pb06 = pin<port_b, 0x6>;
        using pb07 = pin<port_b, 0x7>;
        using pb08 = pin<port_b, 0x8>;
        using pb09 = pin<port_b, 0x9>;
        using pb10 = pin<port_b, 0xA>;
        using pb11 = pin<port_b, 0xB>;
        using pb12 = pin<port_b, 0xC>;
        using pb13 = pin<port_b, 0xD>;
        using pb14 = pin<port_b, 0xE>;
        using pb15 = pin<port_b, 0xF>;

        using pc00 = pin<port_c, 0x0>;
        using pc01 = pin<port_c, 0x1>;
        using pc02 = pin<port_c, 0x2>;
        using pc03 = pin<port_c, 0x3>;
        using pc04 = pin<port_c, 0x4>;
        using pc05 = pin<port_c, 0x5>;
        using pc06 = pin<port_c, 0x6>;
        using pc07 = pin<port_c, 0x7>;
        using pc08 = pin<port_c, 0x8>;
        using pc09 = pin<port_c, 0x9>;
        using pc10 = pin<port_c, 0xA>;
        using pc11 = pin<port_c, 0xB>;
        using pc12 = pin<port_c, 0xC>;
        using pc13 = pin<port_c, 0xD>;
        using pc14 = pin<port_c, 0xE>;
        using pc15 = pin<port_c, 0xF>;

        using pd00 = pin<port_d, 0x0>;
        using pd01 = pin<port_d, 0x1>;
        using pd02 = pin<port_d, 0x2>;
        using pd03 = pin<port_d, 0x3>;
        using pd04 = pin<port_d, 0x4>;
        using pd05 = pin<port_d, 0x5>;
        using pd06 = pin<port_d, 0x6>;
        using pd07 = pin<port_d, 0x7>;
        using pd08 = pin<port_d, 0x8>;
        using pd09 = pin<port_d, 0x9>;
        using pd10 = pin<port_d, 0xA>;
        using pd11 = pin<port_d, 0xB>;
        using pd12 = pin<port_d, 0xC>;
        using pd13 = pin<port_d, 0xD>;
        using pd14 = pin<port_d, 0xE>;
        using pd15 = pin<port_d, 0xF>;

        using pe00 = pin<port_e, 0x0>;
        using pe01 = pin<port_e, 0x1>;
        using pe02 = pin<port_e, 0x2>;
        using pe03 = pin<port_e, 0x3>;
        using pe04 = pin<port_e, 0x4>;
        using pe05 = pin<port_e, 0x5>;
        using pe06 = pin<port_e, 0x6>;
        using pe07 = pin<port_e, 0x7>;
        using pe08 = pin<port_e, 0x8>;
        using pe09 = pin<port_e, 0x9>;
        using pe10 = pin<port_e, 0xA>;
        using pe11 = pin<port_e, 0xB>;
        using pe12 = pin<port_e, 0xC>;
        using pe13 = pin<port_e, 0xD>;
        using pe14 = pin<port_e, 0xE>;
        using pe15 = pin<port_e, 0xF>;

        using pf00 = pin<port_f, 0x0>;
        using pf01 = pin<port_f, 0x1>;
        using pf02 = pin<port_f, 0x2>;
        using pf03 = pin<port_f, 0x3>;
        using pf04 = pin<port_f, 0x4>;
        using pf05 = pin<port_f, 0x5>;
        using pf06 = pin<port_f, 0x6>;
        using pf07 = pin<port_f, 0x7>;
        using pf08 = pin<port_f, 0x8>;
        using pf09 = pin<port_f, 0x9>;
        using pf10 = pin<port_f, 0xA>;
        using pf11 = pin<port_f, 0xB>;
        using pf12 = pin<port_f, 0xC>;
        using pf13 = pin<port_f, 0xD>;
        using pf14 = pin<port_f, 0xE>;
        using pf15 = pin<port_f, 0xF>;

        using pg00 = pin<port_g, 0x0>;
        using pg01 = pin<port_g, 0x1>;
        using pg02 = pin<port_g, 0x2>;
        using pg03 = pin<port_g, 0x3>;
        using pg04 = pin<port_g, 0x4>;
        using pg05 = pin<port_g, 0x5>;
        using pg06 = pin<port_g, 0x6>;
        using pg07 = pin<port_g, 0x7>;
        using pg08 = pin<port_g, 0x8>;
        using pg09 = pin<port_g, 0x9>;
        using pg10 = pin<port_g, 0xA>;
        using pg11 = pin<port_g, 0xB>;
        using pg12 = pin<port_g, 0xC>;
        using pg13 = pin<port_g, 0xD>;
        using pg14 = pin<port_g, 0xE>;
        using pg15 = pin<port_g, 0xF>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart1 = typename ::zoal::arch::stm32f1::usart<0x40013800u, TxSize, RxSize, clock<0, 0x04000>, irq_ctrl<1, 0x20>, pa09, pa10>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart2 = typename ::zoal::arch::stm32f1::usart<0x40004400u, TxSize, RxSize, clock<0x20000, 0>, irq_ctrl<1, 0x40>, pa02, pa03>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart3 = typename ::zoal::arch::stm32f1::usart<0x40004800u, TxSize, RxSize, clock<0x40000, 0>, irq_ctrl<1, 0x80>, pb10, pa11>;

        template<class Controller, class Next>
        using aggregation_link = typename ::zoal::gpio::aggregation_link<Controller, Next>;

        class aggregation_chain_builder {
        protected:
            using link0 = aggregation_link<port_a, ::zoal::gpio::terminator>;
            using link1 = aggregation_link<port_b, link0>;
            using link2 = aggregation_link<port_c, link1>;
            using link3 = aggregation_link<port_d, link2>;
        public:
            using type = aggregation_link<port_e, link3>;
        };

        using gpio_chain = typename aggregation_chain_builder::type;
        using api = ::zoal::gpio::base_api<gpio_chain>;
    };
}}

#endif

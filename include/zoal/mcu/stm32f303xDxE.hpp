#pragma once

#ifndef ZOAL_MCU_SMT32F3XX_HPP
#define ZOAL_MCU_SMT32F3XX_HPP

#include "base_mcu.hpp"
#include "metadata/stm32f303xDxE.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/pwm_connection.hpp"
#include "../arch/cortex/stm32x/port.hpp"
#include "../arch/cortex/stm32x/usart.hpp"
#include "../arch/cortex/stm32f3/spi.hpp"
#include "../arch/cortex/stm32f3/adc.hpp"
#include "../arch/cortex/stm32f3/adc_common_regs.hpp"
#include "../arch/cortex/stm32f3/general_purpose_timer.hpp"
#include "../arch/cortex/stm32x/reset_and_clock_control.hpp"
#include "../arch/cortex/stm32x/bus_clock_control.hpp"
#include "../arch/cortex/stm32x/cfg.hpp"
#include "../arch/cortex/stm32x/mux.hpp"
#include "../arch/cortex/nested_vectored_interrupt_controller.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class stm32f303xDxE : public base_mcu<Frequency, 4> {
    public:
        using nvic = ::zoal::arch::stm32f1::nested_vectored_interrupt_controller<>;
        using rcc = ::zoal::arch::stm32x::reset_and_clock_control<>;

        template<uint32_t Set, uint32_t Clear = ~Set>
        using clock_ahb = ::zoal::arch::stm32x::bus_clock_control<rcc, zoal::arch::bus::cortex_ahb, Set, Clear>;

        template<uint32_t Set, uint32_t Clear = ~Set>
        using clock_apb1 = ::zoal::arch::stm32x::bus_clock_control<rcc, zoal::arch::bus::cortex_apb1, Set, Clear>;

        template<uint32_t Set, uint32_t Clear = ~Set>
        using clock_apb2 = ::zoal::arch::stm32x::bus_clock_control<rcc, zoal::arch::bus::cortex_apb2, Set, Clear>;

        template<uintptr_t Address, class Clock>
        using port = typename ::zoal::arch::stm32x::port<Address, Clock>;

        using port_a = port<0x48000000, clock_ahb<0x000020000>>;
        using port_b = port<0x48000400, clock_ahb<0x000040000>>;
        using port_c = port<0x48000800, clock_ahb<0x000080000>>;
        using port_d = port<0x48000C00, clock_ahb<0x000100000>>;
        using port_e = port<0x48001000, clock_ahb<0x000200000>>;
        using port_f = port<0x48001400, clock_ahb<0x000400000>>;

        using timer2 = zoal::arch::stm32f3::general_purpose_timer<
                0x40000000,
                2,
                clock_apb1<0x000000001>>;
        using timer3 = zoal::arch::stm32f3::general_purpose_timer<
                0x40000400,
                3,
                clock_apb1<0x000000002>>;
        using timer4 = zoal::arch::stm32f3::general_purpose_timer<
                0x40000800,
                4,
                clock_apb1<0x000000004>>;

//        using timer6 = zoal::arch::stm32f3::general_purpose_timer<0x40001000, 5, clock_apb1<0x000000010>>;
//        using timer7 = zoal::arch::stm32f3::general_purpose_timer<0x40002000, 7, clock_apb1<0x000000020>>;

        using adc_common12 = zoal::arch::stm32f3::adc_common_regs<0x50000300u>;
        using adc_common34 = zoal::arch::stm32f3::adc_common_regs<0x50000700u>;

        template<uintptr_t Address, uint8_t N, class CommRegs, class Clock>
        using adc = typename ::zoal::arch::stm32f3::adc<Address, N, CommRegs, Clock>;

        using enable_adc12 = clock_ahb<0x10000000u>;
        using enable_adc34 = clock_ahb<0x20000000u>;
//        using adc12_pll_div6 = options_cfgr2<0x00000130, ~0x000001F0u>;
//        using adc34_pll_div6 = options_cfgr2<0x00002600, ~0x00003E00u>;
//        using adc12_options = ::zoal::arch::stm32x::clock_control_set<enable_adc12, adc12_pll_div6>;
//        using adc34_options = ::zoal::arch::stm32x::clock_control_set<enable_adc34, adc34_pll_div6>;

        using adc_01 = adc<0x50000000u, 1, adc_common12, enable_adc12>;
        using adc_02 = adc<0x50000100u, 2, adc_common12, enable_adc12>;
        using adc_03 = adc<0x50000400u, 3, adc_common34, enable_adc34>;
        using adc_04 = adc<0x50000500u, 4, adc_common34, enable_adc34>;

        template<class Buffer>
        using usart1 = typename ::zoal::arch::stm32x::usart<0x40013800u, 1, Buffer, clock_apb2<0x4000>>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

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

        template<uintptr_t Address, uint32_t RCCMask>
        using spi_controller = typename ::zoal::arch::stm32f3::spi_controller<Address, rcc, RCCMask>;

        template<class Ctrl>
        using spi = typename ::zoal::arch::stm32f3::spi<Ctrl>;

        using spi1 = spi<spi_controller<0x40013000, 0>>;
        using spi2 = spi<spi_controller<0x40003800, 1>>;
        using spi3 = spi<spi_controller<0x40003c00, 2>>;

        using port_chain = typename ::zoal::gpio::chain_builder<port_a, port_b, port_c, port_d, port_e, port_f>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
        using mux = ::zoal::arch::stm32x::mux<api>;
        using cfg = ::zoal::arch::stm32x::cfg<api>;
    };
}}

#endif

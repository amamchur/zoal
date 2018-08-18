#pragma once

#ifndef ZOAL_MCU_SMT32F3XX_HPP
#define ZOAL_MCU_SMT32F3XX_HPP

#include "base_mcu.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/adc_connection.hpp"
#include "../periph/pwm_connection.hpp"
#include "../arch/cortex/stm32f3/port.hpp"
#include "../arch/cortex/stm32f3/spi.hpp"
#include "../arch/cortex/stm32f3/adc.hpp"
#include "../arch/cortex/stm32f3/adc_common_regs.hpp"
#include "../arch/cortex/stm32x/reset_and_clock_controller.hpp"
#include "../arch/cortex/stm32x/peripheral_clock.hpp"
#include "../arch/cortex/stm32x/interrupt_control.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Freq>
    class stm32f303xDxE : public base_mcu<Freq, 4> {
    public:
        using RstClk = ::zoal::arch::stm32x::reset_and_clock_controller<0x40021000>;

        template<uint32_t set, uint32_t clear = ~set>
        using options_ahbenr = ::zoal::stm32x::peripheral_option<RstClk, ::zoal::stm32x::RCCReg::AHBENR, set, clear>;

        template<uint32_t set, uint32_t clear = ~set>
        using options_cfgr2 = ::zoal::stm32x::peripheral_option<RstClk, ::zoal::stm32x::RCCReg::CFGR2, set, clear>;

        template<uint32_t MaskAHBENR>
        using clock = ::zoal::stm32x::peripheral_clock<RstClk, MaskAHBENR, 0, 0>;

        template<uintptr_t Address, class PClock>
        using port = typename ::zoal::gpio::stm32f3::port<Address, PClock>;

        using port_a = port<0x48000000, options_ahbenr<0x000020000>>;
        using port_b = port<0x48000400, options_ahbenr<0x000040000>>;
        using port_c = port<0x48000800, options_ahbenr<0x000080000>>;
        using port_d = port<0x48000C00, options_ahbenr<0x000100000>>;
        using port_e = port<0x48001000, options_ahbenr<0x000200000>>;
        using port_f = port<0x48001400, options_ahbenr<0x000400000>>;

        using adc_common12 = zoal::arch::stm32f3::adc_common_regs<0x50000300>;
        using adc_common34 = zoal::arch::stm32f3::adc_common_regs<0x50000700>;

        template<uintptr_t Address, uint8_t N, class CommRegs, class Clock>
        using adc = typename ::zoal::arch::stm32f3::adc<Address, N, CommRegs, Clock>;

        using enable_adc12 = options_ahbenr<0x10000000>;
        using enable_adc34 = options_ahbenr<0x20000000>;
        using adc12_pll_div6 = options_cfgr2<0x00000130, ~0x000001F0u>;
        using adc34_pll_div6 = options_cfgr2<0x00002600, ~0x00003E00u>;
        using adc12_options = ::zoal::stm32x::peripheral_option_set<enable_adc12, adc12_pll_div6>;
        using adc34_options = ::zoal::stm32x::peripheral_option_set<enable_adc34, adc34_pll_div6>;

        using adc1 = adc<0x50000000, 1, adc_common12, adc12_options>;
        using adc2 = adc<0x50000100, 2, adc_common12, adc12_options>;
        using adc3 = adc<0x50000400, 3, adc_common34, adc34_options>;
        using adc4 = adc<0x50000500, 4, adc_common34, adc34_options>;

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

        template<class P, uint8_t Channel>
        using adc1_connection = typename ::zoal::periph::adc_connection<P, adc1, Channel>;

        template<class P, uint8_t Channel>
        using adc2_connection = typename ::zoal::periph::adc_connection<P, adc2, Channel>;

        template<class P, uint8_t Channel>
        using adc3_connection = typename ::zoal::periph::adc_connection<P, adc3, Channel>;

        template<class P, uint8_t Channel>
        using adc4_connection = typename ::zoal::periph::adc_connection<P, adc3, Channel>;

        using pa00_adc1 = adc1_connection<pa00, 0x01>;
        using pa01_adc1 = adc1_connection<pa01, 0x02>;
        using pa02_adc1 = adc1_connection<pa02, 0x03>;
        using pa03_adc1 = adc1_connection<pa03, 0x04>;
        using pa04_adc2 = adc2_connection<pa04, 0x01>;
        using pa05_adc2 = adc2_connection<pa05, 0x02>;
        using pa06_adc2 = adc2_connection<pa06, 0x03>;
        using pa07_adc2 = adc2_connection<pa07, 0x04>;
        using pb00_adc3 = adc3_connection<pb00, 0x0C>;
        using pb01_adc3 = adc3_connection<pb01, 0x01>;
        using pb02_adc2 = adc2_connection<pb02, 0x0C>;
        using pb12_adc4 = adc4_connection<pb12, 0x03>;
        using pb13_adc3 = adc3_connection<pb13, 0x05>;
        using pb14_adc4 = adc4_connection<pb14, 0x04>;
        using pb15_adc4 = adc4_connection<pb15, 0x05>;
        using pc00_adc2 = adc2_connection<pc00, 0x06>;
        using pc01_adc2 = adc2_connection<pc01, 0x07>;
        using pc04_adc2 = adc2_connection<pc04, 0x05>;
        using pc05_adc2 = adc2_connection<pc05, 0x0B>;
        using pd08_adc4 = adc4_connection<pd08, 0x0C>;
        using pd09_adc4 = adc4_connection<pd09, 0x0D>;
        using pe07_adc3 = adc3_connection<pe07, 0x0D>;
        using pe09_adc3 = adc3_connection<pe09, 0x02>;
        using pe10_adc3 = adc3_connection<pe10, 0x0E>;
        using pe11_adc3 = adc3_connection<pe11, 0x0F>;
        using pe12_adc3 = adc3_connection<pe12, 0x10>;
        using pe13_adc3 = adc3_connection<pe13, 0x03>;
        using pe14_adc4 = adc4_connection<pe14, 0x01>;
        using pe15_adc4 = adc4_connection<pe15, 0x02>;
        using pf04_adc1 = adc1_connection<pf04, 0x05>;

        template<uintptr_t Address, uint32_t RCCMask>
        using spi_controller = typename ::zoal::gpio::stm32f3::spi_controller<Address, RstClk, RCCMask>;

        template<class Ctrl>
        using spi = typename ::zoal::gpio::stm32f3::spi<Ctrl>;

        using spi1 = spi<spi_controller<0x40013000, 0>>;
        using spi2 = spi<spi_controller<0x40003800, 1>>;
        using spi3 = spi<spi_controller<0x40003c00, 2>>;

        template<class Controller, class Next>
        using aggregation_link = typename ::zoal::gpio::aggregation_link<Controller, Next>;

        class aggregator_chain_builder {
        protected:
            using link0 = aggregation_link<port_a, ::zoal::gpio::terminator>;
            using link1 = aggregation_link<port_b, link0>;
            using link2 = aggregation_link<port_c, link1>;
            using link3 = aggregation_link<port_d, link2>;
            using link4 = aggregation_link<port_e, link3>;
        public:
            using type = aggregation_link<port_f, link4>;
        };

        typedef typename aggregator_chain_builder::type gpio_chain;
        using api = ::zoal::gpio::base_api<gpio_chain>;
    };
}}

#endif

#pragma once

#ifndef ZOAL_MCU_SMT32F0XX_HPP
#define ZOAL_MCU_SMT32F0XX_HPP

#include "base_mcu.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../arch/cortex/stm32f0/port.hpp"
#include "zoal/arch/cortex/stm32x/reset_and_clock_control.hpp"
#include "zoal/arch/cortex/stm32x/clock_control.hpp"
#include "../arch/cortex/stm32x/interrupt_control.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class stm32f0xx : public base_mcu<Frequency, 4> {
    public:
        using rcc = typename ::zoal::arch::stm32x::reset_and_clock_control<0x40021000>;

        template<uint32_t Set, uint32_t Clear = ~Set>
        using options_ahbenr = ::zoal::arch::stm32x::clock_control<rcc, ::zoal::arch::stm32x::rcc_register::AHBENR, Set, Clear>;

        template<uintptr_t Address, class Clock>
        using port = typename ::zoal::gpio::stm32f0::port<Address, Clock>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using port_a = port<0x48000000, options_ahbenr<0x000020000>>;
        using port_b = port<0x48000400, options_ahbenr<0x000040000>>;
        using port_c = port<0x48000800, options_ahbenr<0x000080000>>;
        using port_d = port<0x48000C00, options_ahbenr<0x000100000>>;
        using port_e = port<0x48001000, options_ahbenr<0x000200000>>;
        using port_f = port<0x48001400, options_ahbenr<0x000400000>>;

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

        using port_chain = typename ::zoal::gpio::chain_builder<port_a, port_b, port_c, port_d, port_e, port_f>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
    };
}}

#endif

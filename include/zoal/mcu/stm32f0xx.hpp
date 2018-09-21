#pragma once

#ifndef ZOAL_MCU_SMT32F0XX_HPP
#define ZOAL_MCU_SMT32F0XX_HPP

#include "../arch/bus.hpp"
#include "zoal/arch/cortex/stm32x/bus_clock.hpp"
#include "../arch/cortex/stm32x/port.hpp"
#include "../arch/cortex/stm32x/reset_and_clock_control.hpp"
#include "../arch/enable.hpp"
#include "../arch/power.hpp"
#include "../ct/type_list.hpp"
#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "base_mcu.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class stm32f0xx : public base_mcu<Frequency, 4> {
    public:
        using rcc = typename ::zoal::arch::stm32x::reset_and_clock_control<>;

        template<uint32_t Set>
        using clock_ahb = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb, Set>;

        template<uintptr_t Address, class Clock>
        using port = typename ::zoal::arch::stm32x::port<Address, Clock>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using port_a = port<0x48000000, clock_ahb<0x000020000>>;
        using port_b = port<0x48000400, clock_ahb<0x000040000>>;
        using port_c = port<0x48000800, clock_ahb<0x000080000>>;
        using port_d = port<0x48000C00, clock_ahb<0x000100000>>;
        using port_e = port<0x48001000, clock_ahb<0x000200000>>;
        using port_f = port<0x48001400, clock_ahb<0x000400000>>;

        using pa_00 = pin<port_a, 0x0>;
        using pa_01 = pin<port_a, 0x1>;
        using pa_02 = pin<port_a, 0x2>;
        using pa_03 = pin<port_a, 0x3>;
        using pa_04 = pin<port_a, 0x4>;
        using pa_05 = pin<port_a, 0x5>;
        using pa_06 = pin<port_a, 0x6>;
        using pa_07 = pin<port_a, 0x7>;
        using pa_08 = pin<port_a, 0x8>;
        using pa_09 = pin<port_a, 0x9>;
        using pa_10 = pin<port_a, 0xA>;
        using pa_11 = pin<port_a, 0xB>;
        using pa_12 = pin<port_a, 0xC>;
        using pa_13 = pin<port_a, 0xD>;
        using pa_14 = pin<port_a, 0xE>;
        using pa_15 = pin<port_a, 0xF>;

        using pb_00 = pin<port_b, 0x0>;
        using pb_01 = pin<port_b, 0x1>;
        using pb_02 = pin<port_b, 0x2>;
        using pb_03 = pin<port_b, 0x3>;
        using pb_04 = pin<port_b, 0x4>;
        using pb_05 = pin<port_b, 0x5>;
        using pb_06 = pin<port_b, 0x6>;
        using pb_07 = pin<port_b, 0x7>;
        using pb_08 = pin<port_b, 0x8>;
        using pb_09 = pin<port_b, 0x9>;
        using pb_10 = pin<port_b, 0xA>;
        using pb_11 = pin<port_b, 0xB>;
        using pb_12 = pin<port_b, 0xC>;
        using pb_13 = pin<port_b, 0xD>;
        using pb_14 = pin<port_b, 0xE>;
        using pb_15 = pin<port_b, 0xF>;

        using pc_00 = pin<port_c, 0x0>;
        using pc_01 = pin<port_c, 0x1>;
        using pc_02 = pin<port_c, 0x2>;
        using pc_03 = pin<port_c, 0x3>;
        using pc_04 = pin<port_c, 0x4>;
        using pc_05 = pin<port_c, 0x5>;
        using pc_06 = pin<port_c, 0x6>;
        using pc_07 = pin<port_c, 0x7>;
        using pc_08 = pin<port_c, 0x8>;
        using pc_09 = pin<port_c, 0x9>;
        using pc_10 = pin<port_c, 0xA>;
        using pc_11 = pin<port_c, 0xB>;
        using pc_12 = pin<port_c, 0xC>;
        using pc_13 = pin<port_c, 0xD>;
        using pc_14 = pin<port_c, 0xE>;
        using pc_15 = pin<port_c, 0xF>;

        using pd_00 = pin<port_d, 0x0>;
        using pd_01 = pin<port_d, 0x1>;
        using pd_02 = pin<port_d, 0x2>;
        using pd_03 = pin<port_d, 0x3>;
        using pd_04 = pin<port_d, 0x4>;
        using pd_05 = pin<port_d, 0x5>;
        using pd_06 = pin<port_d, 0x6>;
        using pd_07 = pin<port_d, 0x7>;
        using pd_08 = pin<port_d, 0x8>;
        using pd_09 = pin<port_d, 0x9>;
        using pd_10 = pin<port_d, 0xA>;
        using pd_11 = pin<port_d, 0xB>;
        using pd_12 = pin<port_d, 0xC>;
        using pd_13 = pin<port_d, 0xD>;
        using pd_14 = pin<port_d, 0xE>;
        using pd_15 = pin<port_d, 0xF>;

        using pe_00 = pin<port_e, 0x0>;
        using pe_01 = pin<port_e, 0x1>;
        using pe_02 = pin<port_e, 0x2>;
        using pe_03 = pin<port_e, 0x3>;
        using pe_04 = pin<port_e, 0x4>;
        using pe_05 = pin<port_e, 0x5>;
        using pe_06 = pin<port_e, 0x6>;
        using pe_07 = pin<port_e, 0x7>;
        using pe_08 = pin<port_e, 0x8>;
        using pe_09 = pin<port_e, 0x9>;
        using pe_10 = pin<port_e, 0xA>;
        using pe_11 = pin<port_e, 0xB>;
        using pe_12 = pin<port_e, 0xC>;
        using pe_13 = pin<port_e, 0xD>;
        using pe_14 = pin<port_e, 0xE>;
        using pe_15 = pin<port_e, 0xF>;

        using pf_00 = pin<port_f, 0x0>;
        using pf_01 = pin<port_f, 0x1>;
        using pf_02 = pin<port_f, 0x2>;
        using pf_03 = pin<port_f, 0x3>;
        using pf_04 = pin<port_f, 0x4>;
        using pf_05 = pin<port_f, 0x5>;
        using pf_06 = pin<port_f, 0x6>;
        using pf_07 = pin<port_f, 0x7>;
        using pf_08 = pin<port_f, 0x8>;
        using pf_09 = pin<port_f, 0x9>;
        using pf_10 = pin<port_f, 0xA>;
        using pf_11 = pin<port_f, 0xB>;
        using pf_12 = pin<port_f, 0xC>;
        using pf_13 = pin<port_f, 0xD>;
        using pf_14 = pin<port_f, 0xE>;
        using pf_15 = pin<port_f, 0xF>;

        using ports = ::zoal::ct::type_list<port_a, port_b, port_c, port_d, port_e, port_f>;
        using api = ::zoal::gpio::api<ports>;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

#endif

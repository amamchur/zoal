#pragma once

#ifndef ZOAL_MCU_SMT32F0XX_HPP
#define ZOAL_MCU_SMT32F0XX_HPP

#include "../arch/cortex/stm32f1/adc.hpp"
#include "../arch/cortex/stm32f1/port.hpp"
#include "../arch/cortex/stm32f1/usart.hpp"
#include "zoal/arch/cortex/stm32x/bus_clock.hpp"
#include "../arch/cortex/stm32x/mux.hpp"
#include "zoal/arch/cortex/stm32x/rcc.hpp"
#include "../arch/enable.hpp"
#include "../arch/power.hpp"
#include "../ct/type_list.hpp"
#include "../gpio/api.hpp"
#include "../gpio/pin.hpp"
#include "../utils/ms_counter.hpp"
#include "../utils/tool_set.hpp"
#include "base_mcu.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class stm32f1xx : public base_mcu<Frequency, 4> {
    private:
        using rcc = typename ::zoal::arch::stm32x::rcc<>;

        template<uint32_t Set>
        using clock_apb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb1, Set>;

        template<uint32_t Set>
        using clock_apb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb2, Set>;

    public:
        template<uintptr_t Address, class Clock>
        using port = typename ::zoal::arch::stm32f1::port<Address, Clock>;

        template<uintptr_t BaseAddress, uint8_t N, class Clock>
        using adc = typename ::zoal::arch::stm32f1::adc<BaseAddress, N, Clock>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using port_a = port<0x40010800, clock_apb2<0x004>>;
        using port_b = port<0x40010c00, clock_apb2<0x008>>;
        using port_c = port<0x40011000, clock_apb2<0x010>>;
        using port_d = port<0x40011400, clock_apb2<0x020>>;
        using port_e = port<0x40011800, clock_apb2<0x040>>;
        using port_f = port<0x40011c00, clock_apb2<0x080>>;
        using port_g = port<0x40012000, clock_apb2<0x100>>;

        using adc1 = adc<0x40012400, 1, clock_apb2<0x0200>>;
        using adc2 = adc<0x40012400, 2, clock_apb2<0x0400>>;
        using adc3 = adc<0x40013c00, 3, clock_apb2<0x8000>>;

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

        using pg_00 = pin<port_g, 0x0>;
        using pg_01 = pin<port_g, 0x1>;
        using pg_02 = pin<port_g, 0x2>;
        using pg_03 = pin<port_g, 0x3>;
        using pg_04 = pin<port_g, 0x4>;
        using pg_05 = pin<port_g, 0x5>;
        using pg_06 = pin<port_g, 0x6>;
        using pg_07 = pin<port_g, 0x7>;
        using pg_08 = pin<port_g, 0x8>;
        using pg_09 = pin<port_g, 0x9>;
        using pg_10 = pin<port_g, 0xA>;
        using pg_11 = pin<port_g, 0xB>;
        using pg_12 = pin<port_g, 0xC>;
        using pg_13 = pin<port_g, 0xD>;
        using pg_14 = pin<port_g, 0xE>;
        using pg_15 = pin<port_g, 0xF>;

        //        template<uintptr_t TxSize, uintptr_t RxSize>
        //        using usart1 = typename ::zoal::arch::stm32f10x::usart<0x40013800u, TxSize, RxSize, clock_apb2<0x04000>, irq_ctrl<1, 0x20>, pa09, pa10>;
        //
        //        template<uintptr_t TxSize, uintptr_t RxSize>
        //        using usart2 = typename ::zoal::arch::stm32f10x::usart<0x40004400u, TxSize, RxSize, clock_apb1<0x20000>, irq_ctrl<1, 0x40>, pa02, pa03>;
        //
        //        template<uintptr_t TxSize, uintptr_t RxSize>
        //        using usart3 = typename ::zoal::arch::stm32f10x::usart<0x40004800u, TxSize, RxSize, clock_apb1<0x40000>, irq_ctrl<1, 0x80>, pb10, pa11>;

        using ports = ::zoal::ct::type_list<port_a, port_b, port_c, port_d, port_e, port_f, port_g>;
        using api = ::zoal::gpio::api<ports>;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

#endif

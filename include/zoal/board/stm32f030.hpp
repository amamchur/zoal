#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../gpio/api.hpp"
#include "../mcu/stm32f0xx.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class stm32f030 {
    public:
        using mcu = typename mcu::stm32f0xx<Frequency>;

        using pa_00 = typename mcu::pa_00;
        using pa_01 = typename mcu::pa_01;
        using pa_02 = typename mcu::pa_02;
        using pa_03 = typename mcu::pa_03;
        using pa_04 = typename mcu::pa_04;
        using pa_05 = typename mcu::pa_05;
        using pa_06 = typename mcu::pa_06;
        using pa_07 = typename mcu::pa_07;
        using pa_08 = typename mcu::pa_08;
        using pa_09 = typename mcu::pa_09;
        using pa_10 = typename mcu::pa_10;
        using pa_11 = typename mcu::pa_11;
        using pa_12 = typename mcu::pa_12;
        using pa_13 = typename mcu::pa_13;
        using pa_14 = typename mcu::pa_14;
        using pa_15 = typename mcu::pa_15;

        using pb_00 = typename mcu::pb_00;
        using pb_01 = typename mcu::pb_01;
        using pb_02 = typename mcu::pb_02;
        using pb_03 = typename mcu::pb_03;
        using pb_04 = typename mcu::pb_04;
        using pb_05 = typename mcu::pb_05;
        using pb_06 = typename mcu::pb_06;
        using pb_07 = typename mcu::pb_07;

        using pf_00 = typename mcu::pf_00;
        using pf_01 = typename mcu::pf_01;
    };

    using pcb = stm32f030<72000000>;
}

#endif

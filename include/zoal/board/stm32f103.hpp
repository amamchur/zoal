#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../gpio/api.hpp"
#include "../mcu/stm32f1xx.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class stm32f103 {
    public:
        using mcu = zoal::mcu::stm32f1xx<Frequency>;

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
        using pb_08 = typename mcu::pb_08;
        using pb_09 = typename mcu::pb_09;
        using pb_10 = typename mcu::pb_10;
        using pb_11 = typename mcu::pb_11;
        using pb_12 = typename mcu::pb_12;
        using pb_13 = typename mcu::pb_13;
        using pb_14 = typename mcu::pb_14;
        using pb_15 = typename mcu::pb_15;

        using pc_00 = typename mcu::pc_00;
        using pc_01 = typename mcu::pc_01;
        using pc_02 = typename mcu::pc_02;
        using pc_03 = typename mcu::pc_03;
        using pc_04 = typename mcu::pc_04;
        using pc_05 = typename mcu::pc_05;
        using pc_06 = typename mcu::pc_06;
        using pc_07 = typename mcu::pc_07;
        using pc_08 = typename mcu::pc_08;
        using pc_09 = typename mcu::pc_09;
        using pc_10 = typename mcu::pc_10;
        using pc_11 = typename mcu::pc_11;
        using pc_12 = typename mcu::pc_12;
        using pc_13 = typename mcu::pc_13;
        using pc_14 = typename mcu::pc_14;
        using pc_15 = typename mcu::pc_15;

        using pd_00 = typename mcu::pd_00;
        using pd_01 = typename mcu::pd_01;
        using pd_02 = typename mcu::pd_02;
        using pd_03 = typename mcu::pd_03;
        using pd_04 = typename mcu::pd_04;
        using pd_05 = typename mcu::pd_05;
        using pd_06 = typename mcu::pd_06;
        using pd_07 = typename mcu::pd_07;
        using pd_08 = typename mcu::pd_08;
        using pd_09 = typename mcu::pd_09;
        using pd_10 = typename mcu::pd_10;
        using pd_11 = typename mcu::pd_11;
        using pd_12 = typename mcu::pd_12;
        using pd_13 = typename mcu::pd_13;
        using pd_14 = typename mcu::pd_14;
        using pd_15 = typename mcu::pd_15;
    };

    using pcb = stm32f103<72000000>;
}

#endif

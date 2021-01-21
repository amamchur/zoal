#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../mcu/atmega32u4.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class arduino_leonardo {
    public:
        using mcu = mcu::atmega32u4;

        using ard_d00 = typename mcu::pd_02;
        using ard_d01 = typename mcu::pd_03;
        using ard_d02 = typename mcu::pd_01;
        using ard_d03 = typename mcu::pd_00;
        using ard_d04 = typename mcu::pd_04;
        using ard_d05 = typename mcu::pc_06;
        using ard_d06 = typename mcu::pd_07;
        using ard_d07 = typename mcu::pe_06;
        using ard_d08 = typename mcu::pb_04;
        using ard_d09 = typename mcu::pb_05;
        using ard_d10 = typename mcu::pb_06;
        using ard_d11 = typename mcu::pb_07;
        using ard_d12 = typename mcu::pd_06;
        using ard_d13 = typename mcu::pc_07;

        using ard_a00 = typename mcu::pf_07;
        using ard_a01 = typename mcu::pf_06;
        using ard_a02 = typename mcu::pf_05;
        using ard_a03 = typename mcu::pf_04;
        using ard_a04 = typename mcu::pf_01;
        using ard_a05 = typename mcu::pf_00;
    };

#if defined(F_CPU)
    using pcb = arduino_leonardo<F_CPU>;
#endif
}

#endif

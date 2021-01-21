#ifndef ZOAL_BOARD_ARDUINO_ATMAGEXX8_HPP
#define ZOAL_BOARD_ARDUINO_ATMAGEXX8_HPP

#include "../mcu/atmega328p.hpp"

namespace zoal { namespace board {
    template<uint32_t Frequency>
    class arduino_atmegaxx8 {
    public:
        using mcu = mcu::atmega328p;

        using ard_d00 = typename mcu::pd_00;
        using ard_d01 = typename mcu::pd_01;
        using ard_d02 = typename mcu::pd_02;
        using ard_d03 = typename mcu::pd_03;
        using ard_d04 = typename mcu::pd_04;
        using ard_d05 = typename mcu::pd_05;
        using ard_d06 = typename mcu::pd_06;
        using ard_d07 = typename mcu::pd_07;
        using ard_d08 = typename mcu::pb_00;
        using ard_d09 = typename mcu::pb_01;
        using ard_d10 = typename mcu::pb_02;
        using ard_d11 = typename mcu::pb_03;
        using ard_d12 = typename mcu::pb_04;
        using ard_d13 = typename mcu::pb_05;

        using ard_a00 = typename mcu::pc_00;
        using ard_a01 = typename mcu::pc_01;
        using ard_a02 = typename mcu::pc_02;
        using ard_a03 = typename mcu::pc_03;
        using ard_a04 = typename mcu::pc_04;
        using ard_a05 = typename mcu::pc_05;
    };
}}

#endif

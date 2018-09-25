#ifndef ZOAL_BOARD_NUCLEOF303RE_HPP
#define ZOAL_BOARD_NUCLEOF303RE_HPP

#include "../mcu/stm32f303re.hpp"

namespace zoal {
    template<uint32_t HighSpeedExternalOscillator = 8000000, uint8_t PhaseLockedLoop = 9>
    class nucleo_f303re {
    public:
        using mcu = typename mcu::stm32f303re<HighSpeedExternalOscillator, PhaseLockedLoop>;

        using ard_d00 = typename mcu::pa_03;
        using ard_d01 = typename mcu::pa_02;
        using ard_d02 = typename mcu::pa_10;
        using ard_d03 = typename mcu::pb_03;
        using ard_d04 = typename mcu::pb_05;
        using ard_d05 = typename mcu::pb_04;
        using ard_d06 = typename mcu::pb_10;
        using ard_d07 = typename mcu::pa_08;
        using ard_d08 = typename mcu::pa_09;
        using ard_d09 = typename mcu::pc_07;
        using ard_d10 = typename mcu::pb_06;
        using ard_d11 = typename mcu::pa_07;
        using ard_d12 = typename mcu::pa_06;
        using ard_d13 = typename mcu::pa_05;

        using ard_a00 = typename mcu::pa_00;
        using ard_a01 = typename mcu::pa_01;
        using ard_a02 = typename mcu::pa_04;
        using ard_a03 = typename mcu::pb_00;
        using ard_a04 = typename mcu::pc_01;
        using ard_a05 = typename mcu::pc_00;

        using build_in_led = typename mcu::pa_05;
        using build_in_button = typename mcu::pc_13;
    };

    using pcb = nucleo_f303re<>;
}

#endif

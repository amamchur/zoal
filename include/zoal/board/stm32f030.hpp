#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../mcu/stm32f0xx.hpp"
#include "../gpio/base_api.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class stm32f030 {
    public:
        using mcu = typename mcu::stm32f0xx<Frequency>;

        using pa00 = typename mcu::pa00;
        using pa01 = typename mcu::pa01;
        using pa02 = typename mcu::pa02;
        using pa03 = typename mcu::pa03;
        using pa04 = typename mcu::pa04;
        using pa05 = typename mcu::pa05;
        using pa06 = typename mcu::pa06;
        using pa07 = typename mcu::pa07;
        using pa08 = typename mcu::pa08;
        using pa09 = typename mcu::pa09;
        using pa10 = typename mcu::pa10;
        using pa11 = typename mcu::pa11;
        using pa12 = typename mcu::pa12;
        using pa13 = typename mcu::pa13;
        using pa14 = typename mcu::pa14;
        using pa15 = typename mcu::pa15;

        using pb00 = typename mcu::pb00;
        using pb01 = typename mcu::pb01;
        using pb02 = typename mcu::pb02;
        using pb03 = typename mcu::pb03;
        using pb04 = typename mcu::pb04;
        using pb05 = typename mcu::pb05;
        using pb06 = typename mcu::pb06;
        using pb07 = typename mcu::pb07;

        using pf00 = typename mcu::pf00;
        using pf01 = typename mcu::pf01;
    };

    typedef stm32f030<72000000> pcb;
}

#endif

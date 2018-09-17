#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../mcu/stm32f1xx.hpp"
#include "zoal/gpio/api.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class stm32f103 {
    public:
        using mcu = zoal::mcu::stm32f1xx<Frequency>;

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
        using pb08 = typename mcu::pb08;
        using pb09 = typename mcu::pb09;
        using pb10 = typename mcu::pb10;
        using pb11 = typename mcu::pb11;
        using pb12 = typename mcu::pb12;
        using pb13 = typename mcu::pb13;
        using pb14 = typename mcu::pb14;
        using pb15 = typename mcu::pb15;

        using pc00 = typename mcu::pc00;
        using pc01 = typename mcu::pc01;
        using pc02 = typename mcu::pc02;
        using pc03 = typename mcu::pc03;
        using pc04 = typename mcu::pc04;
        using pc05 = typename mcu::pc05;
        using pc06 = typename mcu::pc06;
        using pc07 = typename mcu::pc07;
        using pc08 = typename mcu::pc08;
        using pc09 = typename mcu::pc09;
        using pc10 = typename mcu::pc10;
        using pc11 = typename mcu::pc11;
        using pc12 = typename mcu::pc12;
        using pc13 = typename mcu::pc13;
        using pc14 = typename mcu::pc14;
        using pc15 = typename mcu::pc15;

        using pd00 = typename mcu::pd00;
        using pd01 = typename mcu::pd01;
        using pd02 = typename mcu::pd02;
        using pd03 = typename mcu::pd03;
        using pd04 = typename mcu::pd04;
        using pd05 = typename mcu::pd05;
        using pd06 = typename mcu::pd06;
        using pd07 = typename mcu::pd07;
        using pd08 = typename mcu::pd08;
        using pd09 = typename mcu::pd09;
        using pd10 = typename mcu::pd10;
        using pd11 = typename mcu::pd11;
        using pd12 = typename mcu::pd12;
        using pd13 = typename mcu::pd13;
        using pd14 = typename mcu::pd14;
        using pd15 = typename mcu::pd15;

        using pe00 = typename mcu::pe00;
        using pe01 = typename mcu::pe01;
        using pe02 = typename mcu::pe02;
        using pe03 = typename mcu::pe03;
        using pe04 = typename mcu::pe04;
        using pe05 = typename mcu::pe05;
        using pe06 = typename mcu::pe06;
        using pe07 = typename mcu::pe07;
        using pe08 = typename mcu::pe08;
        using pe09 = typename mcu::pe09;
        using pe10 = typename mcu::pe10;
        using pe11 = typename mcu::pe11;
        using pe12 = typename mcu::pe12;
        using pe13 = typename mcu::pe13;
        using pe14 = typename mcu::pe14;
        using pe15 = typename mcu::pe15;
    };

    using pcb = stm32f103<72000000>;
}

#endif

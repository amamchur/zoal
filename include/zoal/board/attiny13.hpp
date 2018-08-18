#ifndef ZOAL_BOARD_ATTINY13_HPP
#define ZOAL_BOARD_ATTINY13_HPP

#include "../mcu/attiny13.hpp"
#include "../gpio/base_api.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class attiny13 {
    public:
        using mcu = typename mcu::attiny13<Frequency>;

        using pb0 = typename mcu::pb0;
        using pb1 = typename mcu::pb1;
        using pb2 = typename mcu::pb2;
        using pb3 = typename mcu::pb3;
        using pb4 = typename mcu::pb4;
        using pb5 = typename mcu::pb5;

        using pb2_adc0 = typename mcu::pb2_adc0;
        using pb3_adc0 = typename mcu::pb3_adc0;
        using pb4_adc0 = typename mcu::pb4_adc0;
        using pb5_adc0 = typename mcu::pb5_adc0;

        using pb0_timer0 = typename mcu::pb0_timer0;
        using pb1_timer0 = typename mcu::pb1_timer0;
    };

#if defined(F_CPU)
    using pcb = attiny13<F_CPU>;
#endif
}

#endif

#ifndef ZOAL_BOARD_ATTINY2313_HPP
#define ZOAL_BOARD_ATTINY2313_HPP

#include "../mcu/attiny2313.hpp"
#include "../gpio/base_api.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class attiny2313 {
    public:
        using mcu = typename mcu::attiny2313<Frequency>;
    };

#if defined(F_CPU)
    typedef attiny2313<F_CPU> pcb;
#endif
}

#endif

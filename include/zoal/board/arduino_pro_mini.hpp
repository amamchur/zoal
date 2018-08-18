#ifndef ZOAL_BOARD_PRO_MINI_HPP
#define ZOAL_BOARD_PRO_MINI_HPP

#include "arduino_atmegaxx8.hpp"

namespace zoal { namespace board {
    template<uint32_t Frequency>
    class arduino_pro_mini : public board::arduino_atmegaxx8<Frequency> {
    public:
        // 	typedef mcu::c6 	BA06;
        // 	typedef mcu::c7 	BA07;
    };

#if defined(F_CPU)
    typedef arduino_pro_mini<F_CPU> pcb;
#endif
}}

#endif

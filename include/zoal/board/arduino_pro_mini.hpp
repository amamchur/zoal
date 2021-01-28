#ifndef ZOAL_BOARD_PRO_MINI_HPP
#define ZOAL_BOARD_PRO_MINI_HPP

#include "arduino_atmegaxx8.hpp"

namespace zoal { namespace board {
    template<uint32_t Frequency>
    class arduino_pro_mini : public board::arduino_atmegaxx8<Frequency> {
    public:
        // 	using BA06 = mcu::c6 ;
        // 	using BA07 = mcu::c7 ;
    };
}}

#endif

#pragma once

#ifndef ZOAL_BOARD_ARDUINO_UNO_HPP
#define ZOAL_BOARD_ARDUINO_UNO_HPP

#include "arduino_atmegaxx8.hpp"

namespace zoal {
    template<uint32_t Frequency>
    using arduino_uno = board::arduino_atmegaxx8<Frequency>;

#if defined(F_CPU)
	typedef arduino_uno<F_CPU> pcb;
#endif
}

#endif


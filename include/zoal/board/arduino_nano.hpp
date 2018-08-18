#pragma once

#ifndef ZOAL_BOARD_ARDUINO_NANO_HPP
#define ZOAL_BOARD_ARDUINO_NANO_HPP

#include "arduino_atmegaxx8.hpp"

namespace zoal {
    template<uint32_t Frequency>
    using arduino_nano = board::arduino_atmegaxx8<Frequency>;

#if defined(F_CPU)
    typedef arduino_nano<F_CPU> pcb;
#endif
}

#endif

#pragma once

#ifndef ZOAL_PERIPH_TIMER_INTERRUPT_HPP
#define ZOAL_PERIPH_TIMER_INTERRUPT_HPP

namespace zoal::periph {
    enum class timer_interrupt {
        overflow,
        compare_match
    };
}

#endif

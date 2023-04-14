#ifndef ZOAL_MS_TIMER_HPP
#define ZOAL_MS_TIMER_HPP

#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/delay.hpp>

extern volatile uint32_t milliseconds;

using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using delay = zoal::utils::delay<F_CPU, counter>;

#endif

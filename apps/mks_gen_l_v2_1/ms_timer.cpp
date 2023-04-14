#include "ms_timer.hpp"

volatile uint32_t milliseconds = 0;

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

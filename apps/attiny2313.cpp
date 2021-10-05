#include "templates/neo_pixel.hpp"
#include <zoal/mcu/attiny2313a.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint16_t milliseconds = 0;

using mcu = zoal::mcu::attiny2313a;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using logger = zoal::utils::plain_logger<void>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using tools = zoal::utils::tool_set<mcu, F_CPU, counter, logger>;

int main() {
    mcu::power<timer>::on();

//    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();
    mcu::enable<timer>::on();

    zoal::utils::interrupts::on();

    mcu::pd_05::mode<zoal::gpio::pin_mode::output>();
    mcu::pd_05::high();
    while (1) {
        mcu::pd_05::low();
        tools::delay::ms(100);
        mcu::pd_05::high();
        tools::delay::ms(100);
    }
    return 0;
}

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

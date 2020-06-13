#include "templates/blink.hpp"
#include "templates/compile_check.hpp"
#include "templates/ir_remove.hpp"
#include "templates/max72xx.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/neo_pixel.hpp"
#include "templates/tm1637.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <zoal/ic/max72xx.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/input_stream.hpp>
#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/io/rotary_encoder.hpp>
#include <zoal/mcu/attiny2313a.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/shield/uno_lcd.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint16_t milliseconds = 0;

using mcu = zoal::mcu::attiny2313a<F_CPU>;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using logger = zoal::utils::plain_logger<void>;
using irq_handler = counter::handler<mcu::frequency, 64, timer>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;

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
    irq_handler::increment();
}

#include <zoal/board/attiny2313.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/input_stream.hpp>
#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/io/rotary_encoder.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/tool_set.hpp>

#include "templates/blink.hpp"
#include "templates/compile_check.hpp"
#include "templates/ir_remove.hpp"
#include "templates/max72xx.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/neo_pixel.hpp"
#include "templates/tm1637.hpp"
#include "templates/uno_lcd_shield.hpp"

volatile uint16_t milliseconds = 0;

using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using ms_timer = zoal::pcb::mcu::timer0;
using irq_handler = counter::handler<zoal::pcb::mcu::frequency, 64, ms_timer>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, counter>;
using mcu = zoal::pcb::mcu;

int main() {
//    ms_timer::mode<zoal::periph::timer_mode::fast_pwm_8bit>();
//    ms_timer::select_clock_source<prescaler>();
//    ms_timer::enable_overflow_interrupt();
    zoal::utils::interrupts::on();

    mcu::pd5::mode<zoal::gpio::pin_mode::output>();
    mcu::pd5::high();
    while (1) {
        mcu::pd5::low();
        tools::delay::ms(100);
        mcu::pd5::high();
        tools::delay::ms(100);
    }
    return 0;
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}
#include <avr/eeprom.h>

#include <zoal/board/arduino_uno.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/input_stream.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/rotary_encoder.hpp>
#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>

#include "templates/neo_pixel.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/blink.hpp"
#include "templates/uno_lcd_shield.hpp"
#include "templates/usart_output.hpp"
#include "templates/max72xx.hpp"
#include "templates/ir_remove.hpp"
#include "templates/tm1637.hpp"
#include "templates/compile_check.hpp"

volatile uint32_t milliseconds = 0;

using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using ms_timer = zoal::pcb::mcu::timer0;
using prescaler = zoal::utils::prescaler_le<ms_timer, 64>::result;
using irq_handler = counter::handler<zoal::pcb::mcu::frequency, prescaler::value, ms_timer>;
using usart_config = zoal::periph::usart_config<zoal::pcb::mcu::frequency, 115200>;
using usart = zoal::pcb::mcu::usart0<4, 8>;
using logger = zoal::utils::plain_logger<usart, zoal::utils::log_level::info>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, counter, logger>;
using delay = tools::delay;
using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb>;
using app4 = usart_output<usart, usart_config, tools, zoal::pcb::ard_d13>;
using app5 = max72xx<tools, zoal::pcb::mcu::mosi0, zoal::pcb::mcu::sclk0, zoal::pcb::ard_d10>;
using app6 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using check = compile_check<app0, app1, app2, app3, app4, app5, app6, app7>;

app1 app;

void initialize() {
    usart::setup<usart_config>();

    ms_timer::reset();
    ms_timer::mode<zoal::periph::timer_mode::fast_pwm_8bit>();
    ms_timer::select_clock_source<prescaler>();
    ms_timer::enable_overflow_interrupt();
    zoal::utils::interrupts::on();
}

using sspi = zoal::gpio::tx_software_spi<zoal::pcb::ard_d10, zoal::pcb::ard_d08>;
using max7219 = zoal::ic::max72xx<sspi, zoal::pcb::ard_d09>;
zoal::ic::max72xx_data<1> matrix;

int main() {
//    check::check();

    initialize();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    max7219::init(1);
    matrix.clear();
    long value = 0;
    while (true) {
        matrix.print(zoal::data::segment7::gfed_ascii, (long) value);
        max7219::display(matrix);
        value++;
        ::delay::ms(50);
    }
    return 0;
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART_RX_vect) {
    usart::handle_rx_irq();
}

ISR(USART_UDRE_vect) {
    usart::handle_tx_irq();
}

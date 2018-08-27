#include <avr/eeprom.h>

#include <zoal/board/arduino_mega.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/board/arduino_atmegaxx8.hpp>

#include "templates/keypad_mega.hpp"
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

using ms_timer = typename pcb::mcu::timer0;
using prescaler = zoal::utils::prescaler_le<ms_timer, 64>::result;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using irq_handler = typename counter::handler<pcb::mcu::frequency, prescaler::value, ms_timer>;
using usart_config = pcb::mcu::usart_config<115200>;
using usart = pcb::mcu::usart0<64, 8>;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::info>;
using tools = utils::tool_set<pcb::mcu, counter, logger>;
using app0 = neo_pixel<tools, pcb::ard_d13>;
using app1 = multi_function_shield<tools, pcb>;
using app2 = blink<tools, pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, pcb>;
using app4 = usart_output<usart, usart_config, tools, pcb::ard_d13>;
using app5 = max72xx<tools, pcb::mcu::mosi0, pcb::mcu::sclk0, pcb::ard_d10>;
using app6 = ir_remove<pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, pcb::ard_d10, pcb::ard_d11>;
using app8 = keypad<keypad_type::keypad_5x4, tools>;
using check = compile_check<app0, app1, app2, app3, app4, app5, app6, app7, app8>;

app8 app;

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART0_RX_vect) {
    usart::handle_rx_irq();
}

ISR(USART0_UDRE_vect) {
    usart::handle_tx_irq();
}

int main() {
    check::check();
    usart::setup<usart_config>();

    ms_timer::reset();
    ms_timer::select_clock_source<prescaler>();
    ms_timer::enable_overflow_interrupt();
    zoal::utils::interrupts::on();

    logger::info() << "Started ATmega2560";

    app.init();
    app.run();

//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wmissing-noreturn"
//    while (true) {
//    }
    return 0;
//#pragma clang diagnostic pop
}
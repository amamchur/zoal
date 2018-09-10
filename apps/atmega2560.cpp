#include "templates/blink.hpp"
#include "templates/compile_check.hpp"
#include "templates/ir_remove.hpp"
#include "templates/keypad_mega.hpp"
#include "templates/max72xx.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/neo_pixel.hpp"
#include "templates/tm1637.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <avr/eeprom.h>
#include <zoal/board/arduino_mega.hpp>
#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using ms_timer = typename mcu::timer_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using irq_handler = typename counter::handler<mcu::frequency, 64, ms_timer>;
using usart = mcu::usart0<zoal::data::rx_tx_buffer<8, 8>>;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::info>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb>;
using app5 = max72xx<tools, mcu::mosi0, mcu::sclk0, zoal::pcb::ard_d10>;
using app6 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using app8 = keypad<keypad_type::keypad_5x4, tools>;
using check = compile_check<app0, app1, app2, app3, app5, app6, app7, app8>;

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

    usart::power_on();
    mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01>::on();
    mcu::cfg::usart<usart, 115200>::apply();
    usart::enable();

    ms_timer::power_on();
    mcu::cfg::timer<ms_timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    ms_timer::enable();
    mcu::irq::timer<ms_timer>::enable_overflow_interrupt();
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

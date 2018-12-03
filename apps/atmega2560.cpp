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
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using timer = typename mcu::timer_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using irq_handler = typename counter::handler<mcu::frequency, 64, timer>;
using log_usart = mcu::usart_00;
using usart_01_tx_buffer = zoal::periph::tx_ring_buffer<log_usart, 64>;

using adc = mcu::adc_00;
using logger = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::info>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb, mcu::adc_00>;
//using app5 = max72xx<tools, mcu::mosi0, mcu::sclk0, zoal::pcb::ard_d10>;
using app6 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using app8 = keypad<tools, keypad_type::keypad_4x4>;
using check = compile_check<app0, app1, app2, app3, app6, app7, app8>;

app8 app;

void initialize_hardware() {
    mcu::power<log_usart, timer, adc>::on();

    mcu::mux::usart<log_usart, mcu::pe_00, mcu::pe_01, mcu::pe_02>::on();
    mcu::cfg::usart<log_usart, 115200>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::cfg::adc<adc>::apply();

    mcu::enable<log_usart, timer, adc>::on();

    zoal::utils::interrupts::on();
}

int main() {
    using zoal::gpio::pin_mode;

    initialize_hardware();

    logger::info() << "Started ATmega2560";

    app8::gpio_cfg();
    app.init();
    app.run();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        app.run_once();
    }
    return 0;
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART0_RX_vect) {
}

ISR(USART0_UDRE_vect) {
    log_usart::tx_handler<usart_01_tx_buffer>();
}

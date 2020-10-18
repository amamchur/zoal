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
#include <zoal/shield/uno_lcd.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using timer = typename mcu::timer_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using irq_handler = typename counter::handler<mcu::frequency, 64, timer>;
using usart = mcu::usart_00;
using tx_buffer = usart::default_tx_buffer<16>;
using rx_buffer = usart::default_rx_buffer<16>;

using adc = mcu::adc_00;
using logger = zoal::utils::terminal_logger<tx_buffer, zoal::utils::log_level::info>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb, mcu::adc_00>;
//using app5 = max72xx<tools, mcu::mosi0, mcu::sclk0, zoal::pcb::ard_d10>;
using app4 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app5 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using app6 = keypad<tools, keypad_type::keypad_4x4>;
using check = compile_check<app0, app1, app2, app3, app4, app5, app6>;

app6 app;

void initialize_hardware() {
    mcu::power<usart, timer, adc>::on();

    mcu::mux::usart<usart, mcu::pe_00, mcu::pe_01, mcu::pe_02>::connect();
    mcu::cfg::usart<usart, 115200>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::cfg::adc<adc>::apply();

    mcu::enable<usart, timer, adc>::on();

    zoal::utils::interrupts::on();
}

int main() {
    using zoal::gpio::pin_mode;

    initialize_hardware();

    logger::info() << "Started ATmega2560";

//    app6::gpio_cfg();
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
    usart::rx_handler<rx_buffer>();
}

ISR(USART0_UDRE_vect) {
    usart::tx_handler<tx_buffer>();
}

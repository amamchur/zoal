#include "templates/uno_lcd_shield.hpp"
#include "templates/multi_function_shield.hpp"

#include <avr/eeprom.h>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = zoal::pcb::mcu::timer_00;
using irq_handler = counter::handler<zoal::pcb::mcu::frequency, 64, timer>;
using usart = mcu::usart_01<zoal::data::rx_tx_buffer<8, 8>>;
using adc = mcu::adc_00;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, counter, logger>;
using delay = tools::delay;

//using App = uno_lcd_shield<tools, zoal::pcb, mcu::adc_00>;
//using Keypad = App::shield::keypad;

using App = multi_function_shield<tools, zoal::pcb>;

App app;
//uint16_t lcd_buttons_balue[App::shield::button_count] __attribute__((section(".eeprom"))) = {637, 411, 258, 101, 0};

void initialize_hardware() {
    mcu::power<usart, timer, adc>::on();

    mcu::mux::usart<usart, zoal::pcb::ard_d00, zoal::pcb::ard_d01, mcu::pd_05>::on();
    mcu::cfg::usart<usart, 115200>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::cfg::adc<adc>::apply();

    mcu::enable<usart, timer, adc>::on();

    zoal::utils::interrupts::on();
}

int main() {
    initialize_hardware();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
//    logger::info() << "----- Started -----";

//    eeprom_read_block(Keypad::values, lcd_buttons_balue, sizeof(Keypad::values));
    App::gpio_cfg();
    app.init();
//    eeprom_write_block(Keypad::values, lcd_buttons_balue, sizeof(Keypad::values));

    while (true) {
        app.run_once();
    }
    return 0;
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART1_RX_vect) {
    usart::handle_rx_irq();
}

ISR(USART1_UDRE_vect) {
    usart::handle_tx_irq();
}

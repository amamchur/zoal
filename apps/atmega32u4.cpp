#include "templates/uno_lcd_shield.hpp"

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
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using ms_timer = zoal::pcb::mcu::timer_00;
using irq_handler = counter::handler<zoal::pcb::mcu::frequency, 64, ms_timer>;
using usart = mcu::usart_01<zoal::data::rx_tx_buffer<8, 8>>;

using logger = zoal::utils::plain_logger<usart, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, counter, logger>;
using delay = tools::delay;

using App = uno_lcd_shield<tools, zoal::pcb>;
using Keypad = App::shield::keypad;

App app;
uint16_t ButtonValues[App::shield::button_count] __attribute__((section(".eeprom"))) = {637, 411, 258, 101, 0};

void initializeHardware() {
    usart::power_on();
    mcu::mux::usart<usart, zoal::pcb::ard_d00, zoal::pcb::ard_d01, mcu::pd_05>::on();
    mcu::cfg::usart<usart, 115200>::apply();
    usart::enable();

    ms_timer::power_on();
    mcu::cfg::timer<ms_timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    ms_timer::enable();
    mcu::irq::timer<ms_timer>::enable_overflow_interrupt();
    zoal::utils::interrupts::on();
}

int main() {
    initializeHardware();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    logger::clear();
    logger::info() << "----- Started -----";

    eeprom_read_block(Keypad::values, ButtonValues, sizeof(Keypad::values));
    app.init();
    eeprom_write_block(Keypad::values, ButtonValues, sizeof(Keypad::values));

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

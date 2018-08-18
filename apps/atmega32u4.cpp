#include <avr/eeprom.h>

#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>

#include "templates/uno_lcd_shield.hpp"

using namespace zoal;
using namespace zoal::gpio;
using namespace zoal::mcu;
using namespace zoal::io;

volatile uint32_t milliseconds = 0;

using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using ms_timer = pcb::mcu::timer0;
using prescaler = zoal::utils::prescaler_le<ms_timer, 64>::result;
using irq_handler = counter::handler <pcb::mcu::frequency, prescaler::value, ms_timer>;

using usart_config = zoal::periph::usart_config<pcb::mcu::frequency, 115200>;
using usart = pcb::mcu::usart0<32, 8>;

using logger = zoal::utils::plain_logger<usart, zoal::utils::log_level::trace>;
using tools = utils::tool_set<pcb::mcu, counter, logger>;
using delay = tools::delay;

using App = uno_lcd_shield<tools, pcb>;
using Keypad = App::shield::keypad;

App app;
uint16_t ButtonValues[App::shield::button_count] __attribute__((section (".eeprom"))) = {637, 411, 258, 101, 0};

void initializeHardware() {
    usart::setup<usart_config>();
    ms_timer::reset();
    ms_timer::mode<zoal::periph::timer_mode::fast_pwm_8bit>();
    ms_timer::select_clock_source<prescaler>();
    ms_timer::enable_overflow_interrupt();
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

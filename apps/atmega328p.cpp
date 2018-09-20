#include "templates/blink.hpp"
#include "templates/compile_check.hpp"
#include "templates/ir_remove.hpp"
#include "templates/max72xx.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/neo_pixel.hpp"
#include "templates/tm1637.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <avr/eeprom.h>
#include <zoal/arch/avr/atmega/spi.hpp>
#include <zoal/arch/avr/port.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/input_stream.hpp>
#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/io/rotary_encoder.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/helpers.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using irq_handler = counter::handler<mcu::frequency, 64, timer>;
using usart = mcu::usart_00<zoal::data::rx_tx_buffer<8, 8>>;
using adc = mcu::adc_00;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = tools::delay;
using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb, mcu::adc_00>;
//using app5 = max72xx<tools, mcu::mosi0, mcu::sclk0, zoal::pcb::ard_d10>;
using app6 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using check = compile_check<app0, app1, app2, app3, app6, app7>;

using keypad = typename app3::shield::keypad;
using lcd = typename app3::shield::lcd;

app3 app;

uint16_t lcd_buttons_values[app3::shield::button_count] __attribute__((section(".eeprom"))) = {637, 411, 258, 101, 0};

void initialize_hardware() {
    mcu::power<usart, timer, adc>::on();

    mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::on();
    mcu::cfg::usart<usart, 115200>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::cfg::adc<adc>::apply();

    mcu::enable<usart, timer, adc>::on();

    zoal::utils::interrupts::on();
}

void initialize_application() {
    eeprom_read_block(keypad::values, lcd_buttons_values, sizeof(keypad::values));
    app3::gpio_cfg();
    app.init();
    eeprom_write_block(keypad::values, lcd_buttons_values, sizeof(keypad::values));
}

int main() {
    initialize_hardware();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    //    initialize_application();

#if 1
    using spi = mcu::spi_00;
    mcu::cfg::spi<spi, 2>::apply();
    mcu::mux::spi<spi, mcu::pb_03, mcu::pb_04, mcu::pb_05, mcu::pb_02>::on();
    mcu::enable<spi>::on();

    using max = zoal::ic::max72xx<spi, zoal::pcb::ard_d08>;
#else
    using sw_spi = zoal::periph::tx_software_spi<zoal::pcb::ard_d11, zoal::pcb::ard_d13>;
    sw_spi::enable();

    using max = zoal::ic::max72xx<sw_spi, zoal::pcb::ard_d08>;
#endif

    using matrix_type = zoal::ic::max72xx_data<1>;
    matrix_type matrix;

    max::init(matrix_type::devices);

    uint32_t value = 0;
    while (true) {
        matrix.clear();
        auto end = zoal::utils::radix<16>::split(value, &matrix.data[0][0]);
        if (end == &matrix.data[0][0]) {
            end++;
        }

        zoal::utils::apply(zoal::data::segment7::gfed_hex, &matrix.data[0][0], end);
        max::display(matrix);
//        delay::ms<100>();
        value++;
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

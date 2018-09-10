#include <avr/eeprom.h>

#include <zoal/board/arduino_uno.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/helpers.hpp>
#include <zoal/data/rx_tx_buffer.hpp>
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
#include "templates/max72xx.hpp"
#include "templates/ir_remove.hpp"
#include "templates/tm1637.hpp"
#include "templates/compile_check.hpp"

#include "../atmega328p.hpp"

volatile uint32_t milliseconds = 0;

//using mcu = zoal::mcu::atmega328p<16000000>; //zoal::pcb::mcu;
using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using ms_timer = mcu::timer_00;
using irq_handler = counter::handler<mcu::frequency, 64, ms_timer>;
using usart = mcu::usart_00<zoal::data::rx_tx_buffer<8, 8>>;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = tools::delay;
using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb>;
//using app5 = max72xx<tools, mcu::mosi0, mcu::sclk0, zoal::pcb::ard_d10>;
using app6 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using check = compile_check<app0, app1, app2, app3, app6, app7>;

app3 app;

void initialize() {
    usart::power_on();
    ms_timer::power_on();

    mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01>::on();
    mcu::cfg::usart<usart, 115200>::apply();
    mcu::cfg::timer<ms_timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();

    mcu::irq::timer<ms_timer>::enable_overflow_interrupt();

    usart::enable();
    ms_timer::enable();

    zoal::utils::interrupts::on();
}

using sspi = zoal::gpio::tx_software_spi<zoal::pcb::ard_d10, zoal::pcb::ard_d08>;
using max7219 = zoal::ic::max72xx<sspi, zoal::pcb::ard_d09>;
zoal::ic::max72xx_data<1> matrix;

int main() {
    initialize();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
//
//    max7219::init(1);
//    matrix.clear();

    zoal::utils::list_iterator<zoal::metadata::atmega_timer_clock_dividers>::for_each([](size_t index, uintptr_t value){
        logger::info() << "Index: " << index << " Value: " << value;
    });

    mcu::adc_00::power_on();
    mcu::cfg::adc<mcu::adc_00>::apply();
    mcu::adc_00::enable();

    app.init();
    while (true) {
        app.run_once();
//        logger::info() << value++;
////        matrix.print(zoal::data::segment7::gfed_ascii, (long) value);
////        max7219::display(matrix);
////        value++;
//        ::delay::ms(1000);
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

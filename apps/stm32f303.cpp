#include "stm32f30x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zoal/board/nucleo_f303re.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/ic/ws2812.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/arch/cortex/stm32f3/adc.hpp>
#include <zoal/arch/cortex/stm32x/clock_control.hpp>
#include <zoal/arch/cortex/semihosting_transport.hpp>
#include <zoal/periph/adc_connection.hpp>

volatile uint32_t milliseconds_counter = 0;

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

using mcu = zoal::pcb::mcu;
using logger = zoal::utils::plain_logger<void, zoal::utils::log_level ::info>;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = typename tools::delay;
using connection = mcu::pa00_adc1;
using shield = zoal::shields::uno_lcd_shield<tools, zoal::pcb>;
using lcd_output_stream = zoal::io::output_stream<shield::lcd>;

lcd_output_stream stream;

#pragma GCC diagnostic push

int test_adc() {
    using namespace zoal::io;

    connection::pin::port::enable();
    connection::adc::setup();
    connection::on();

    uint16_t prev = 0xFFFF;
    while (1) {
        auto result = connection::read();
        if (prev != result) {
            prev = result;
            stream << pos(0, 0) << "Value: " << result << "    ";
        }
        tools::delay::ms(5);
    }
}

void handler(uint8_t button, zoal::io::button_event e) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    stream << zoal::io::pos(1, 0) << "Button: " << button;
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);

    logger::info() << "Hello NucleoF303RE!";
    logger::info() << "System clock: " << SystemCoreClock << " Hz";

    shield::init();
    stream << "NucleoF303RE" << zoal::io::pos(1, 0) << "Started!";

    test_adc();

    while (1) {
//        test_adc();
        shield::handle_keypad(&handler);
    }

    return 0;
}

#pragma GCC diagnostic pop

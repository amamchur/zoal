#include <zoal/mcu/stm32f303re.hpp>

#include "stm32f30x.h"
#include "templates/compile_check.hpp"
#include "templates/ir_remove.hpp"
#include "templates/multi_function_shield.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zoal/board/nucleo_f303re.hpp>
#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/ic/ws2812.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

//using mcu = zoal::pcb::mcu;
using mcu = zoal::mcu::stm32f303_rd_re<>;
using usart_01 = mcu::usart_01<zoal::data::rx_tx_buffer<8, 8>>;
using usart_02 = mcu::usart_02<zoal::data::rx_tx_buffer<8, 8>>;
using usart_03 = mcu::usart_03<zoal::data::rx_tx_buffer<8, 8>>;
using logger = zoal::utils::terminal_logger<usart_01, zoal::utils::log_level::trace>;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = typename tools::delay;
using shield = zoal::shields::uno_lcd_shield<tools, zoal::pcb, mcu::adc_01>;
using lcd_output_stream = zoal::io::output_stream<shield::lcd>;

//using app1 = multi_function_shield<tools, zoal::pcb>;
//using check = compile_check<app1>;

//app1 app;
//lcd_output_stream stream;

#pragma GCC diagnostic push

//void handler(uint8_t button, zoal::io::button_event e) {
//    if (e != zoal::io::button_event::press) {
//        return;
//    }
//
//    stream << zoal::io::pos(1, 0) << "Button: ";// << button;
//}

void initTimer() {
    zoal::pcb::build_in_led::port::power_on();
    zoal::pcb::build_in_led::mode<zoal::gpio::pin_mode::output>();

    mcu::timer_02::power_on();
    mcu::timer_02::prescaler<7200>();
    mcu::timer_02::period<5000>();
    mcu::timer_02::enable_interrupt<zoal::periph::timer_interrupt::overflow>();
    mcu::timer_02::enable();
}

void init_usart() {
    mcu::power<usart_01>::on();

    mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09, mcu::pa_08>::on();
    mcu::cfg::usart<usart_01, 115200>::apply();

    //    mcu::mux::usart<usart_02, mcu::pa_10, mcu::pa_09, mcu::pa_08>::on();
    //    mcu::cfg::usart<usart_02, 115200>::apply();

    //    mcu::mux::usart<usart_03, mcu::pa_10, mcu::pa_09, mcu::pa_08>::on();
    //    mcu::cfg::usart<usart_03, 115200>::apply();

    mcu::enable<usart_01>::on();

    NVIC_EnableIRQ(USART1_IRQn);
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);
    zoal::utils::interrupts::on();

    init_usart();

    logger::clear();
    logger::info() << "----- Started!!! -----";
    logger::trace() << zoal::io::hex << "USART1: 0x" << (uintptr_t)(USART1);
    logger::trace() << zoal::io::hex << "USART2: 0x" << (uintptr_t)(USART2);
    logger::trace() << zoal::io::hex << "USART3: 0x" << (uintptr_t)(USART3);

    int counter = 0;
    while (1) {
        logger::info() << "counter: " << counter++;
        delay::ms(3000);
    }

    return 0;
}

#pragma GCC diagnostic pop

extern "C" void USART1_IRQHandler(void) {
    usart_01::handleIrq();
}

extern "C" void USART2_IRQHandler(void) {
    usart_02::handleIrq();
}

extern "C" void USART3_IRQHandler(void) {
    usart_03::handleIrq();
}

extern "C" void SysTick_Handler() {
    milliseconds_counter++;
}

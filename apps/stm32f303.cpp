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
#include <zoal/mcu/stm32f303re.hpp>
#include <zoal/mem/accessor.hpp>
#include <zoal/periph/rx_ring_buffer.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/shield/uno_lcd.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::mcu::stm32f303re<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = zoal::periph::tx_ring_buffer<usart_01, 64>;
using usart_02_tx_buffer = zoal::periph::tx_ring_buffer<usart_02, 64>;
using usart_03_tx_buffer = zoal::periph::tx_ring_buffer<usart_03, 64>;

using logger_01 = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using logger_02 = zoal::utils::terminal_logger<usart_02_tx_buffer, zoal::utils::log_level::trace>;
using logger_03 = zoal::utils::terminal_logger<usart_03_tx_buffer, zoal::utils::log_level::trace>;

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter, logger_01>;
using delay = typename tools::delay;
using shield = zoal::shield::uno_lcd<tools, zoal::pcb, mcu::adc_01>;

#pragma GCC diagnostic push

void init_hardware() {
    mcu::power<usart_01, usart_02, mcu::port_a, mcu::port_b>::on();
    mcu::power<usart_02>::on();

    mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::on();
    mcu::cfg::usart<usart_01, 115200>::apply();

    mcu::mux::usart<usart_02, mcu::pa_15, mcu::pb_03>::on();
    mcu::cfg::usart<usart_02, 115200>::apply();

    mcu::enable<usart_01, usart_02>::on();

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);

    zoal::utils::interrupts::on();
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);

    init_hardware();

    logger_01::info() << "USART2 native: " << USART2;
    logger_01::info() << "USART2   zoal: " << (void *)usart_02::address;

    uint8_t v = 'A';

    while (1) {
        logger_01::info() << "USART1";
        logger_02::info() << "USART2";
        delay::ms(1000);
    }

    //    USART_SendData(USART1, output[output_index++]);

    return 0;
}

#pragma GCC diagnostic pop

extern "C" void USART1_IRQHandler(void) {
    usart_01::tx_handler<usart_01_tx_buffer>();
}

extern "C" void USART2_IRQHandler(void) {
    usart_01::tx_handler<usart_02_tx_buffer>();
}

extern "C" void SysTick_Handler() {
    milliseconds_counter++;
}

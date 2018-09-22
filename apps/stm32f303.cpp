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
#include <zoal/periph/rx_buffer.hpp>
#include <zoal/periph/tx_buffer.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::mcu::stm32f303_rd_re<>;
using usart = mcu::usart_01<zoal::data::rx_tx_buffer<8, 8>>;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::trace>;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = typename tools::delay;
using shield = zoal::shields::uno_lcd_shield<tools, zoal::pcb, mcu::adc_01>;
using lcd_output_stream = zoal::io::output_stream<shield::lcd>;

#pragma GCC diagnostic push

void initTimer() {
    zoal::pcb::build_in_led::port::power_on();
    zoal::pcb::build_in_led::mode<zoal::gpio::pin_mode::output>();

    mcu::timer_02::power_on();
    mcu::timer_02::prescaler<7200>();
    mcu::timer_02::period<5000>();
    mcu::timer_02::enable_interrupt<zoal::periph::timer_interrupt::overflow>();
    mcu::timer_02::enable();
}

void init_hardware() {
    mcu::power<usart>::on();
    usart::power_on();
    mcu::port_a::power_on();

    mcu::mux::usart<usart, mcu::pa_10, mcu::pa_09, mcu::pa_08>::on();
    mcu::cfg::usart<usart, 115200>::apply();

    mcu::enable<usart>::on();

    NVIC_EnableIRQ(USART1_IRQn);

    zoal::utils::interrupts::on();
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);

    init_hardware();

#if 1
    mcu::api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pa_00, mcu::pa_01, mcu::pa_02>();
    mcu::pa_00::high();
    mcu::pb_01::high();
    mcu::pc_02::high();
#else
    //    GPIO_InitTypeDef GPIO_InitStruct;
    //    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    //    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    //    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    //    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    //    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_SetBits(GPIOA, 1);

    GPIO_SetBits(GPIOA, 0);
    GPIO_SetBits(GPIOA, 2);
#endif

    logger::info() << "----- Started!!! -----";

    int counter = 0;
    while (1) {
        logger::info() << "counter: " << counter++;
        delay::ms(3000);
    }

    return 0;
}

#pragma GCC diagnostic pop

extern "C" void USART1_IRQHandler(void) {
    usart::handleIrq();
    //    usart_01::tx_handler<tx_buffer>();
    //    usart_01::rx_handler<rx_buffer>();
}

extern "C" void SysTick_Handler() {
    milliseconds_counter++;
}

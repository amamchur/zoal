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
#include <zoal/arch/cortex/stm32x/bus_clock_control.hpp>
#include <zoal/arch/cortex/semihosting_transport.hpp>
#include <zoal/periph/adc_connection.hpp>
#include <zoal/mcu/atmega_640_1280_2560.hpp>
#include <zoal/mcu/stm32f1xx.hpp>

#include "templates/multi_function_shield.hpp"
#include "templates/ir_remove.hpp"
#include "templates/compile_check.hpp"

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::pcb::mcu;
using usart = mcu::usart1<32, 32>;
using logger = zoal::utils::terminal_logger<usart, zoal::utils::log_level::trace>;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = typename tools::delay;
using connection = mcu::pa00_adc1;
using shield = zoal::shields::uno_lcd_shield<tools, zoal::pcb>;
using lcd_output_stream = zoal::io::output_stream<shield::lcd>;

using app1 = multi_function_shield<tools, zoal::pcb>;
using check = compile_check<app1>;

app1 app;
lcd_output_stream stream;

#pragma GCC diagnostic push

void handler(uint8_t button, zoal::io::button_event e) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    stream << zoal::io::pos(1, 0) << "Button: ";// << button;
}

void initTimer() {
    zoal::pcb::build_in_led::port::power_on();
    zoal::pcb::build_in_led::mode<zoal::gpio::pin_mode::output>();

    mcu::timer2::power_on();
    mcu::timer2::prescaler<7200>();
    mcu::timer2::period<5000>();
    mcu::timer2::enable_interrupt<zoal::periph::timer_interrupt::overflow>();
    mcu::timer2::enable();
}

void USART1_Init() {
    usart::power_on();
    mcu::mux::usart<usart, mcu::pa10, mcu::pa09, mcu::pa08>::on();
    mcu::cfg::usart<usart, 57600>::apply();
    usart::enable();

    NVIC_EnableIRQ(USART1_IRQn);
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);
    zoal::utils::interrupts::on();

    USART1_Init();

    //    initTimer();
//
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    NVIC_EnableIRQ(TIM2_IRQn);
//
    logger::clear();
    logger::info() << "----- Started!!! -----";
    logger::trace() << "logger::trace";
    logger::debug() << "logger::debug";
    logger::info() << "logger::info";
    logger::warn() << "logger::warn";
    logger::error() << "logger::error";

    using uc = mcu::cfg::usart<usart, 57600>;
#define CR1_CLEAR_MASK            ((uint32_t)(USART_CR1_M | USART_CR1_PCE | \
                                              USART_CR1_PS | USART_CR1_TE | \
                                              USART_CR1_RE))
    logger::info() << "CR1_CLEAR_MASK: 0x" << zoal::io::hex << CR1_CLEAR_MASK;
    logger::info() << "c1_clear: 0x" << zoal::io::hex << uc::c1_clear;
    logger::info() << "c1_set: 0x" << zoal::io::hex << uc::c1_set;

//    usart::write('A');
//    usart::write('A');
//    usart::write('A');
//    usart::write('\r');
//    usart::write('\n');
//    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//    USART1_print("USART1_print\n\n");

    int counter = 0;
    while (1) {
        logger::info() << "counter: " << counter++;
        delay::ms(1000);
    }

    return 0;
}

#pragma GCC diagnostic pop

#if 0

extern "C" void USART1_EXTI25_IRQHandler(void) {
    if (USART1_writeidx - USART1_readidx == 0) {
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        return;
    }
    USART_SendData(USART1,
                   USART1_ringbuf[(USART1_readidx++) & (RINGBUF_SIZE - 1)]);
}

#else

extern "C" void USART1_EXTI25_IRQHandler(void) {
    usart::handleIrq();
}

#endif

extern "C" void SysTick_Handler() {
    milliseconds_counter++;
}

//extern "C" void TIM2_IRQHandler() {
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//
//    zoal::pcb::build_in_led::toggle();
//}

#include "stm32f0xx.h"

#include <zoal/mcu/stm32f030f4px.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

extern volatile uint32_t uwTick;

using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using mcu = zoal::mcu::stm32f030f4px; // 48 MHz

using tty_usart = mcu::usart_01;
using tools = zoal::utils::tool_set<mcu, 48000000, counter, void>;
using delay = tools::delay;

extern "C" [[noreturn]] void zoal_main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);

    tty_usart::clock_on();
    mcu::port_a::clock_on();

    using usart_01_cfg = zoal::periph::usart_115200<48000000>;
    mcu::mux::usart<tty_usart, mcu::pa_03, mcu::pa_02>::on();
    mcu::cfg::usart<tty_usart, usart_01_cfg>::apply();
//    mcu::enable<tty_usart>::on();

    //NVIC_EnableIRQ(USART1_IRQn);
    zoal::utils::interrupts::on();

    mcu::pa_04::mode<pin_mode::output>();

    while (true) {
        mcu::pa_04::low();
        ::delay::ms(500);

        mcu::pa_04::high();
        ::delay::ms(500);
    }
}

extern "C" void USART1_IRQHandler() {
}

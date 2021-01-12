#include "stm32f0xx.h"

#include <zoal/mcu/stm32f030f4px.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

extern volatile uint32_t uwTick;

using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using mcu = zoal::mcu::stm32f030f4px<8000000, 6>; // 48 MHz

using usart_01 = mcu::usart_01;
using tools = zoal::utils::tool_set<mcu, counter, void>;
using delay = tools::delay;

extern "C" [[noreturn]] void zoal_main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);

    usart_01::power_on();
    mcu::port_a::power_on();

    mcu::mux::usart<usart_01, mcu::pa_03, mcu::pa_02>::on();
    mcu::cfg::usart<usart_01, 115200>::apply();
//    mcu::enable<usart_01>::on();

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

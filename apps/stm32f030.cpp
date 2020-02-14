#include "stm32f0xx.h"

#include <zoal/mcu/stm32f030f4px.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using mcu = zoal::mcu::stm32f030f4px<8000000, 6>; // 48 MHz

using usart_01 = mcu::usart_01;
using usart_01_tx_buffer = usart_01::default_tx_buffer<64>;
//using usart_01_tx_buffer = zoal::periph::tx_ring_buffer<usart_01, 64>;
using logger_01 = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;

using tools = zoal::utils::tool_set<mcu, counter, logger_01>;
using delay = tools::delay;

int main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);
    usart_01::power_on();
    mcu::port_a::power_on();

    mcu::mux::usart<usart_01, mcu::pa_03, mcu::pa_02>::on();
    mcu::cfg::usart<usart_01, 115200>::apply();
    mcu::enable<usart_01>::on();

    NVIC_EnableIRQ(USART1_IRQn);
    zoal::utils::interrupts::on();

    mcu::pa_04::mode<pin_mode::output>();

    logger_01::info() << "--- Started ---";
    while (true) {
        logger_01::info() << "Loop";

        mcu::pa_04::low();
        ::delay::ms(500);

        mcu::pa_04::high();
        ::delay::ms(500);
    }

    return 0;
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler_v2<usart_01_tx_buffer>();
}

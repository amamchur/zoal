#include "stm32f10x.h"

#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/mcu/stm32f103c8.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::mcu::stm32f103c8<>;
using usart_01 = mcu::usart_01<zoal::data::rx_tx_buffer<16, 16>>;
using usart_02 = mcu::usart_02<zoal::data::rx_tx_buffer<16, 16>>;
using usart_03 = mcu::usart_03<zoal::data::rx_tx_buffer<16, 16>>;

using logger_01 = zoal::utils::terminal_logger<usart_01, zoal::utils::log_level::trace>;
using logger_02 = zoal::utils::terminal_logger<usart_02, zoal::utils::log_level::trace>;
using logger_03 = zoal::utils::terminal_logger<usart_03, zoal::utils::log_level::trace>;

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter>;
using delay = tools::delay;

int main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);

    mcu::power<usart_01, usart_02, usart_03, mcu::port_a, mcu::port_b, mcu::port_c>::on();
    mcu::cfg::usart<usart_01, 115200>::apply();
    mcu::cfg::usart<usart_02, 115200>::apply();
    mcu::cfg::usart<usart_03, 115200>::apply();

    mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::on();
    mcu::mux::usart<usart_02, mcu::pa_03, mcu::pa_02>::on();
    mcu::mux::usart<usart_03, mcu::pb_11, mcu::pb_10>::on();
//    mcu::mux::usart<usart_03, mcu::pa_10, mcu::pa_09>::on();


    usart_01::enable();
    usart_02::enable();
    usart_03::enable();

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(USART3_IRQn);

    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();

    int counter = 0;
    while (true) {
        logger_03::info() << counter++;
        mcu::pc_13::toggle();
        ::delay::ms(1000);
    }

    return 0;
}

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

extern "C" void USART1_IRQHandler() {
    usart_01::handleIrq();
}

extern "C" void USART2_IRQHandler() {
    usart_02::handleIrq();
}

extern "C" void USART3_IRQHandler() {
    usart_03::handleIrq();
}

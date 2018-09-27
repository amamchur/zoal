#include "stm32f10x.h"

#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/mcu/stm32f103c8.hpp>
#include <zoal/periph/rx_buffer.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::mcu::stm32f103c8<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = zoal::periph::tx_ring_buffer<usart_01, 64>;
using usart_02_tx_buffer = zoal::periph::tx_ring_buffer<usart_02, 64>;
using usart_03_tx_buffer = zoal::periph::tx_ring_buffer<usart_03, 1024>;

using logger_01 = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using logger_02 = zoal::utils::terminal_logger<usart_02_tx_buffer, zoal::utils::log_level::trace>;
using logger_03 = zoal::utils::terminal_logger<usart_03_tx_buffer, zoal::utils::log_level::trace>;

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter>;
using delay = tools::delay;

#define DEMCR (*((volatile uint32_t *)0xE000EDFC))
#define DWT_CTRL (*(volatile uint32_t *)0xe0001000)
#define CYCCNTENA (1 << 0)
#define DWT_CYCCNT ((volatile uint32_t *)0xE0001004)
#define CPU_CYCLES *DWT_CYCCNT

__attribute__((noinline)) void noinline_shift(uint32_t q) {
    asm volatile("1:	asrs 	%[count],     #1		\n"
                 "   	bne     1b			    \n"
                 :
                 : [count] "r"(q)
                 : "r0", "cc");
}

template<int32_t Value>
void do_test() {
    uint32_t diff = 0;
    uint32_t overhead = 0;
    {
        zoal::utils::interrupts ni(false);
        CPU_CYCLES = 0;
        DWT_CTRL |= CYCCNTENA;
        DWT_CTRL &= ~CYCCNTENA;
        overhead = CPU_CYCLES;
    }
    /*
     * 0 - 7
     * 1 - 7
     * 2 - 9
     * 3 - 15
     */
    {
        zoal::utils::interrupts ni(false);
        CPU_CYCLES = 0;
        DWT_CTRL |= CYCCNTENA;
        zoal::utils::nop<Value>::place();
        DWT_CTRL &= ~CYCCNTENA;
        diff = CPU_CYCLES - overhead;
    }

    logger_03::info() << "v: " << Value << "; ops: " << diff;
}

int main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);

    mcu::power<usart_01, usart_02, usart_03, mcu::port_a, mcu::port_b, mcu::port_c>::on();

    mcu::cfg::usart<usart_01, 115200>::apply();
    mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::on();

    mcu::cfg::usart<usart_02, 115200>::apply();
    mcu::mux::usart<usart_02, mcu::pa_03, mcu::pa_02>::on();

    mcu::cfg::usart<usart_03, 115200>::apply();
    mcu::mux::usart<usart_03, mcu::pb_11, mcu::pb_10>::on();

    usart_01::enable();
    usart_02::enable();
    usart_03::enable();

    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(USART3_IRQn);

    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();

    int counter = 0;

    logger_03::info() << "--- Start ---";

    using np = zoal::utils::nop<33>;
    logger_03::info() << "Loops: " << np::loops << "; ops: " << (np::loops * 9);
    logger_03::info() << "Rest : " << np::rest;

    while (true) {
        logger_03::info() << "-";
        do_test<31>();
        do_test<42>();
        do_test<53>();
        do_test<64>();
        do_test<75>();
        do_test<86>();
        do_test<97>();
        counter++;
        ::delay::ms(3000);
    }

    return 0;
}

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler<usart_01_tx_buffer>();
}

extern "C" void USART2_IRQHandler() {
    usart_01::tx_handler<usart_02_tx_buffer>();
}

extern "C" void USART3_IRQHandler() {
    usart_03::tx_handler<usart_03_tx_buffer>();
}

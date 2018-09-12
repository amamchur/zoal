#include "stm32f0xx.h"
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/board/stm32f030.hpp>

volatile uint32_t milliseconds_counter = 0;

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using mcu = zoal::pcb::mcu;
using tools = zoal::utils::tool_set<mcu, counter>;
using delay = tools::delay;

int main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);
    mcu::pa_04::port::power_on();
    mcu::pa_04::mode<pin_mode::output>();

    while (true) {
        mcu::pa_04::low();
        ::delay::ms(500);

        mcu::pa_04::high();
        ::delay::ms(500);
    }

    return 0;
}
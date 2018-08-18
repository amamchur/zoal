#include "stm32f0xx.h"
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/board/stm32f030.hpp>

using namespace zoal;
using namespace zoal::gpio;
using namespace zoal::mcu;

volatile uint32_t milliseconds_counter = 0;

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, counter>;
using delay = tools::delay;

int main() {
    SysTick_Config(SystemCoreClock / 1000);
    pcb::pa04::port::enable();
    pcb::pa04::mode<pin_mode::output>();
    pcb::pa04::high();

    while (true) {
        pcb::pa04::low();
        ::delay::ms(500);

        pcb::pa04::high();
        ::delay::ms(500);
    }

    return 0;
}
#include "stm32f10x.h"
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/board/stm32f103.hpp>

using namespace zoal;
using namespace zoal::gpio;
using namespace zoal::mcu;

volatile uint32_t milliseconds_counter = 0;

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<pcb::mcu, counter>;
using delay = tools::delay;

int main() {
    SysTick_Config(SystemCoreClock / 1000);
    pcb::pc13::port::enable();
    pcb::pc13::mode<pin_mode::output>();
    pcb::pc13::high();

    while (true) {
        pcb::pc13::low();
        ::delay::ms(500);

        pcb::pc13::high();
        ::delay::ms(500);
    }

    return 0;
}

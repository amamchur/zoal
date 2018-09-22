#include "stm32f10x.h"

#include <zoal/board/stm32f103.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter>;
using delay = tools::delay;

int main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);
    mcu::api::power_on<mcu::pc_13>();
    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();

    while (true) {
        mcu::pc_13::low();
        ::delay::ms(100);

        mcu::pc_13::high();
        ::delay::ms(100);
    }

    return 0;
}

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

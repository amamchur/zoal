#include "stm32f0xx.h"

#include <zoal/board/stm32f030.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

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

    asm volatile("nop            \n");
    asm volatile("nop            \n");
    asm volatile("nop            \n");

    //    mcu::api::low<mcu::pa_00, mcu::pa_01>();

    mcu::api::merge<mcu::api::low<mcu::pa_00>,
                    mcu::api::low<mcu::pa_01>,
                    mcu::api::low<mcu::pa_02>,
                    mcu::api::high<mcu::pa_00>,
                    mcu::api::high<mcu::pa_01>,
                    mcu::api::high<mcu::pa_02>>();

    asm volatile("nop            \n");
    asm volatile("nop            \n");
    asm volatile("nop            \n");

    //    SysTick_Config(SystemCoreClock / 1000);
    //    mcu::pa_04::port::power_on();
    //    mcu::pa_04::mode<pin_mode::output>();
    //
    //    while (true) {
    //        mcu::pa_04::low();
    //        ::delay::ms(500);
    //
    //        mcu::pa_04::high();
    //        ::delay::ms(500);
    //    }

    return 0;
}

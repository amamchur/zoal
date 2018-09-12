#include "stm32f10x.h"

#include <zoal/board/stm32f103.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter>;
using delay = tools::delay;

int main__() {
    using namespace zoal::gpio;

    using a = mcu::api::low<mcu::pa00>;
    using b = mcu::api::low<mcu::pa01>;
    using c = mcu::api::low<mcu::pa00, mcu::pa01>;
    using m = merge_actions<b, c>;
    m::apply();

    //    b::apply();
    c::apply();
    //    mcu::api::mode<pin_mode::output,
    //            mcu::pa00,
    //            mcu::pa01,
    //            mcu::pa02,
    //            mcu::pa03,
    //            mcu::pa04,
    //            mcu::pa05,
    //            mcu::pa06,
    //            mcu::pb07
    //    >::apply();
    return 0;
}

int main() {
    using namespace zoal::gpio;

    SysTick_Config(SystemCoreClock / 1000);
    mcu::api::power_on<mcu::pc13>::apply();
    mcu::pc13::mode<zoal::gpio::pin_mode::output>();

    while (true) {
        mcu::pc13::low();
        ::delay::ms(100);

        mcu::pc13::high();
        ::delay::ms(100);
    }

    return 0;
}

extern "C" void SysTick_Handler(void) {
    milliseconds_counter++;
}

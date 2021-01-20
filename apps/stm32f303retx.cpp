#include "stm32f3xx_hal.h"

#include <zoal/board/nucleo_f303re.hpp>
#include <zoal/mcu/stm32f303retx.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

using mcu = zoal::mcu::stm32f303retx;
using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using tools = zoal::utils::tool_set<mcu, 72000000, counter, void>;
using delay = typename tools::delay;
using pcb = zoal::pcb;

extern "C" [[noreturn]] void zoal_main() {
    using api = zoal::gpio::api;
    using led = pcb::build_in_led;

    api::optimize<
        //
        api::clock_on<led::port>,
        api::mode<zoal::gpio::pin_mode::output, led>
        //
        >();

    while (true) {
        led::low();
        delay::ms(100);

        led::high();
        delay::ms(100);
    }
}

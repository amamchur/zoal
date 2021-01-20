#include <zoal/mcu/stm32f303vctx.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

#include "stm32f3xx_hal.h"

using mcu = zoal::mcu::stm32f303vctx;
using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using tools = zoal::utils::tool_set<mcu, 72000000, counter, void>;
using delay = typename tools::delay;

extern "C" [[noreturn]] void zoal_main() {
    while (true) {
    }
}

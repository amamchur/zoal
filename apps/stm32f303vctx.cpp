#include <zoal/mcu/stm32f303vctx.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

#include "stm32f3xx_hal.h"

using mcu = zoal::mcu::stm32f303vctx<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = usart_01::default_tx_buffer<64>;
using usart_02_tx_buffer = usart_02::default_tx_buffer<64>;
using usart_03_tx_buffer = usart_03::default_tx_buffer<64>;

using logger_01 = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using logger_02 = zoal::utils::terminal_logger<usart_02_tx_buffer, zoal::utils::log_level::trace>;
using logger_03 = zoal::utils::terminal_logger<usart_03_tx_buffer, zoal::utils::log_level::trace>;

using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using tools = zoal::utils::tool_set<mcu, counter, logger_01>;
using delay = typename tools::delay;

extern "C" [[noreturn]] void zoal_main() {
//    mcu::pe
    while (1) {
        mcu::pe_09::low();
        delay::ms(100);

        mcu::pe_09::high();
        delay::ms(100);
    }
}

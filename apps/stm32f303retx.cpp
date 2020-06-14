#include "stm32f3xx_hal.h"

#include <zoal/board/nucleo_f303re.hpp>
#include <zoal/mcu/stm32f303retx.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

using mcu = zoal::mcu::stm32f303retx<>;
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
using pcb = zoal::pcb;

extern "C" [[noreturn]] void zoal_main() {
    using api = zoal::gpio::api;
    using led = pcb::build_in_led;

    api::optimize<
        //
        api::power_on<led::port>,
        api::mode<zoal::gpio::pin_mode::output, led>
        //
        >::apply();

    while (1) {
        led::low();
        delay::ms(100);

        led::high();
        delay::ms(100);
    }
}

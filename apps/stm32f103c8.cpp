#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/periph/rx_ring_buffer.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/utils/cmsis_os2/delay.hpp>
#include <zoal/utils/cmsis_os2/ms_counter.hpp>

using mcu = zoal::mcu::stm32f103c8tx<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = usart_01::default_tx_buffer<16>;

//using usart_01_tx_buffer = zoal::periph::tx_ring_buffer<usart_01, 64>;
using usart_02_tx_buffer = zoal::periph::tx_ring_buffer<usart_02, 64>;
using usart_03_tx_buffer = zoal::periph::tx_ring_buffer<usart_03, 1024>;

//using logger_01 = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
//using logger_02 = zoal::utils::terminal_logger<usart_02_tx_buffer, zoal::utils::log_level::trace>;
//using logger_03 = zoal::utils::terminal_logger<usart_03_tx_buffer, zoal::utils::log_level::trace>;

using counter = zoal::utils::cmsis_os2::ms_counter;
using delay = zoal::utils::cmsis_os2::delay<mcu>;

extern "C" [[noreturn]] void zoal_default_thread() {
    mcu::power<mcu::port_a, mcu::port_b, mcu::port_c>::on();
    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();

    constexpr int delay = 500;
    for (;;) {
        mcu::pc_13::high();
        ::delay::ms(delay);

        mcu::pc_13::low();
        ::delay::ms(delay);
    }
}

extern "C" [[noreturn]] void zoal_main() {
    using namespace zoal::gpio;

    mcu::power<mcu::port_a, mcu::port_b, mcu::port_c>::on();
    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();

    constexpr int delay = 100;

    while (true) {
        mcu::pc_13::high();
        ::delay::ms(delay);

        mcu::pc_13::low();
        ::delay::ms(delay);
    }
}

#include "gpio.h"
#include "stm32f1xx_hal.h"

#include <zoal/data/ring_buffer.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/usart_transmitter.hpp>

using mcu = zoal::mcu::stm32f103c8tx;
using usart_01 = mcu::usart_01;
using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<72000000, counter>;

using api = zoal::gpio::api;
using user_led = mcu::pc_13;
using rx_buffer_type = zoal::data::ring_buffer<uint8_t, 16>;
using tx_buffer_type = zoal::data::ring_buffer<uint8_t, 16>;
using usart_tx_type = zoal::utils::usart_transmitter<usart_01, tx_buffer_type>;
using usart_tx_stream_type = zoal::io::output_stream<usart_tx_type>;

rx_buffer_type rx_buffer;
tx_buffer_type tx_buffer;
usart_tx_type usart_tx(tx_buffer);
usart_tx_stream_type stream(usart_tx);

extern "C" void zoal_init() {
    using usart_01_cfg = zoal::periph::usart_115200<72000000>;
    using usart_mux = mcu::mux::usart<usart_01, mcu::pb_07, mcu::pb_06>;
    using usart_cfg = mcu::cfg::usart<usart_01, usart_01_cfg>;

    api::optimize<
        //
        usart_mux::periph_clock_on,
        usart_cfg::periph_clock_on>();
    api::optimize<api::disable<usart_01>>();
    api::optimize<
        //
        usart_mux::connect,
        usart_cfg::apply
        //
        >();
    api::optimize<api::enable<usart_01>>();

    usart_01::enable_rx();

    HAL_NVIC_SetPriority(USART1_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

[[noreturn]] void zoal_run() {
    uint32_t counter = 0;
    while (true) {
        stream << counter++ << "\r\n";
        delay::ms(1000);
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    zoal_init();
    zoal_run();
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler([](uint8_t &value) { return tx_buffer.pop_front(value); });
    usart_01::rx_handler([](uint8_t value) { rx_buffer.push_back(value); });
}

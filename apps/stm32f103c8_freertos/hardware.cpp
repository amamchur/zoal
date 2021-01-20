#include "hardware.hpp"

#include "stm32f1xx_hal.h"

#include <zoal/periph/usart.hpp>

zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer(1);
zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer(1);

usart_01_tx_transport transport;
tx_stream_type tx_stream(transport);

void zoal_init_hardware() {
    using api = zoal::gpio::api;
    using usart_01_cfg = zoal::periph::usart_115200<72000000>;
    using usart_mux = mcu::mux::usart<usart_01, mcu::pb_07, mcu::pb_06>;
    using usart_cfg = mcu::cfg::usart<usart_01, usart_01_cfg>;

    // Enable bus clock for peripherals
    api::optimize<
        //
        usart_mux::periph_clock_on,
        usart_cfg::periph_clock_on
        //
        >();

    // Disable peripherals before configuration
    api::optimize<api::disable<usart_01>>();

    // Configuring everything
    api::optimize<
        //
        usart_mux::connect,
        usart_cfg::apply,
        user_led::gpio_cfg,
        keypad_type::gpio_cfg,
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12, mcu::pb_13>>();

    // Enable peripherals after configuration
    api::optimize<api::enable<usart_01>>();

    HAL_NVIC_SetPriority(USART1_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    usart_01::enable_rx();
}

void usart_01_tx_transport::send_byte(uint8_t value) {
    tx_stream_buffer.send(value, portMAX_DELAY);
    usart_01 ::enable_tx();
}

void usart_01_tx_transport::send_data(const void *data, size_t size) {
    auto ptr = reinterpret_cast<const char *>(data);
    while (size > 0) {
        auto sent = tx_stream_buffer.send(ptr, size, 0);
        usart_01 ::enable_tx();
        size -= sent;
        ptr += sent;
    }
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler([](uint8_t &value) { return tx_stream_buffer.receive_isr(&value, 1) > 0; });
    usart_01::rx_handler([](uint8_t byte) { rx_stream_buffer.send_isr(byte); });
}

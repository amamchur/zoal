#include "hardware.hpp"

#include "stm32f3xx_hal.h"

#include <zoal/periph/usart.hpp>

i2c_req_dispatcher_type i2c_req_dispatcher;
zoal::periph::i2c_request &request = i2c_req_dispatcher.request;

usart_debug_tx_transport transport;
tx_stream_type tx_stream(transport);

void zoal_init_hardware() {
    using api = zoal::gpio::api;
    using usart_01_cfg = zoal::periph::usart_115200<72000000>;
    using usart_mux = mcu::mux::usart<debug_usart, mcu::pb_07, mcu::pb_06>;
    using usart_cfg = mcu::cfg::usart<debug_usart, usart_01_cfg>;

    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    debug_usart::enable_rx();
}

void usart_debug_tx_transport::send_byte(uint8_t value) {
    txs.send(value, portMAX_DELAY);
    debug_usart::enable_tx();
}

void usart_debug_tx_transport::send_data(const void *data, size_t size) {
    auto ptr = reinterpret_cast<const char *>(data);
    while (size > 0) {
        auto sent = txs.send(ptr, size, 0);
        debug_usart::enable_tx();
        size -= sent;
        ptr += sent;
    }
}

extern "C" void USART3_IRQHandler() {
    debug_usart::tx_handler([](uint8_t &value) { return transport.txs.receive_isr(&value, 1) > 0; });
    debug_usart::rx_handler([](uint8_t byte) { transport.rxs.send_isr(byte); });
}

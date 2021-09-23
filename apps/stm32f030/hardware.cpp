#include "hardware.hpp"

tx_buffer_type tx_buffer;
rx_buffer_type rx_buffer;
usart_tx_type usart_tx(tx_buffer);
usart_tx_stream_type stream(usart_tx);

extern "C" void USART1_IRQHandler() {
    usart::tx_handler([](uint8_t &value) { return tx_buffer.pop_front(value); });
    usart::rx_handler([](uint8_t byte) { rx_buffer.push_back(byte); });
}

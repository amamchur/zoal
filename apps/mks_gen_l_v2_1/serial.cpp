#include "serial.hpp"

zoal::data::ring_buffer<uint8_t, 256> rx_buffer;
usart_tx_type usart_tx;
usart_tx_stream_type stream(usart_tx);

ISR(USART0_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART0_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_type::tx_buffer.pop_front(value); });
}

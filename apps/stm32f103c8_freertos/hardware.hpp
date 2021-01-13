#ifndef ZOAL_STM32F103C8_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F103C8_FREERTOS_HARDWARE_HPP

#include "board.hpp"

#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/mem/reserve_mem.hpp>

using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer;

class usart_01_tx_transport {
public:
    static void send_byte(uint8_t value);
    static void send_data(const void* data, size_t size);
};

using tx_stream_type = zoal::io::output_stream<usart_01_tx_transport>;
extern tx_stream_type tx_stream;

void zoal_init_hardware();

#endif

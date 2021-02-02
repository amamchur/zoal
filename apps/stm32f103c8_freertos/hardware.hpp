#ifndef ZOAL_STM32F103C8_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F103C8_FREERTOS_HARDWARE_HPP

#include "board.hpp"

#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/io/button.hpp>

using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer;

using user_button_1_type =  zoal::io::button<TickType_t, mcu::pb_12>;
using user_button_2_type = zoal::io::button<TickType_t, mcu::pb_13>;

class usart_01_tx_transport {
public:
    static void send_byte(uint8_t value);
    static void send_data(const void* data, size_t size);
};

using tx_stream_type = zoal::io::output_stream<usart_01_tx_transport>;
extern tx_stream_type tx_stream;

void zoal_init_hardware();

#endif

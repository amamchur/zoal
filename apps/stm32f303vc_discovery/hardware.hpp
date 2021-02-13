#ifndef ZOAL_STM32F303_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F303_FREERTOS_HARDWARE_HPP

#include "board.hpp"

#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/mem/reserve_mem.hpp>

using led_03 = zoal::gpio::active_high<mcu::pe_09>;
using led_04 = zoal::gpio::active_high<mcu::pe_08>;
using led_05 = zoal::gpio::active_high<mcu::pe_10>;
using led_06 = zoal::gpio::active_high<mcu::pe_15>;
using led_07 = zoal::gpio::active_high<mcu::pe_11>;
using led_08 = zoal::gpio::active_high<mcu::pe_14>;
using led_09 = zoal::gpio::active_high<mcu::pe_12>;
using led_10 = zoal::gpio::active_high<mcu::pe_13>;

using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;

class usart_debug_tx_transport {
public:
    void send_byte(uint8_t value);
    void send_data(const void *data, size_t size);

    zoal::mem::reserve_mem<stream_buffer_type, 32> rxs{1};
    zoal::mem::reserve_mem<stream_buffer_type, 32> txs{1};
};

using tx_stream_type = zoal::io::output_stream<usart_debug_tx_transport>;
extern usart_debug_tx_transport transport;
extern tx_stream_type tx_stream;

void zoal_init_hardware();

#endif

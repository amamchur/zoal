#ifndef ZOAL_STM32F303_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F303_FREERTOS_HARDWARE_HPP

#include "board.hpp"

#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/freertos/mutex.hpp>
#include <zoal/freertos/scoped_lock.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>

using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;

class usart_debug_tx_transport {
public:
    void send_byte(uint8_t value);
    void send_data(const void *data, size_t size);

    zoal::mem::reserve_mem<stream_buffer_type, 32> rxs{1};
    zoal::mem::reserve_mem<stream_buffer_type, 32> txs{1};

    using mutex_type = zoal::freertos::mutex<zoal::freertos::freertos_allocation_type::static_mem>;
    using scoped_lock = zoal::freertos::scoped_lock<mutex_type>;
    mutex_type mutex;
};

using tx_stream_type = zoal::io::output_stream<usart_debug_tx_transport>;
extern usart_debug_tx_transport tx_target;
extern tx_stream_type tx_stream;

void zoal_init_hardware();

using i2c_01 = mcu::i2c_01;
using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c_01, sizeof(void *) * 8>;
extern i2c_req_dispatcher_type i2c_req_dispatcher;
extern zoal::periph::i2c_request &request;

#endif

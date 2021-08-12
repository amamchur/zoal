#ifndef ZOAL_STM32F103C8_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F103C8_FREERTOS_HARDWARE_HPP

#include "board.hpp"

#include <zoal/freertos/event_group.hpp>
#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/ic/at24cxx.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/i2c_scanner.hpp>

using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer;

using user_button_1_type = zoal::io::button<TickType_t, mcu::pb_12>;
using user_button_2_type = zoal::io::button<TickType_t, mcu::pb_13>;

class usart_01_tx_transport {
public:
    static void send_byte(uint8_t value);
    static void send_data(const void *data, size_t size);
};

using usart_tx_stream_type = zoal::io::output_stream<usart_01_tx_transport>;
extern usart_tx_stream_type tx_stream;

using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c_02, sizeof(void *) * 8>;
extern i2c_req_dispatcher_type i2c_dispatcher;
extern zoal::periph::i2c_request &request;

constexpr uint32_t i2c_event = 1 << 0;
constexpr uint32_t usart_event = 1 << 1;
constexpr uint32_t all_hardware_events = i2c_event | usart_event;
extern zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> hardware_events;

auto constexpr buffer_mem_type = zoal::periph::device_buffer_type::shared_mem;
extern zoal::ic::ds3231<buffer_mem_type> clock;
extern zoal::ic::at24cxx<buffer_mem_type> eeprom;
extern zoal::utils::i2c_scanner scanner;

void zoal_init_hardware();

#endif

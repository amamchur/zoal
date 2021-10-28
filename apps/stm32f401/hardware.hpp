#ifndef ZOAL_STM32F401_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F401_FREERTOS_HARDWARE_HPP

#include "stm32f4xx_hal.h"
#include "zoal/ic/w25qxx.hpp"

#include <zoal/freertos/event_group.hpp>
#include <zoal/freertos/mutex.hpp>
#include <zoal/freertos/scoped_lock.hpp>
#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/ic/at24cxx.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/mcu/stm32f401ccux.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/cmsis_os2/delay.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/ms_counter.hpp>

using mcu = zoal::mcu::stm32f401ccux;
using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::cmsis_os2::delay<84000000>;
using tty_usart = mcu::usart_01;

using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer;
extern zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer;

class usart_01_tx_transport {
public:
    static void send_byte(uint8_t value);
    static void send_data(const void *data, size_t size);
};

using usart_tx_stream_type = zoal::io::output_stream<usart_01_tx_transport>;
extern usart_tx_stream_type tx_stream;

using tx_stream_mutex_type = zoal::freertos::mutex<zoal::freertos::freertos_allocation_type::static_mem>;
using scoped_lock = zoal::freertos::scoped_lock<tx_stream_mutex_type>;
extern tx_stream_mutex_type tx_stream_mutex;

constexpr EventBits_t i2c_event = 1 << 0;
constexpr EventBits_t usart_event = 1 << 1;
constexpr EventBits_t all_hardware_events = i2c_event | usart_event;
extern zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> hardware_events;

using sensor_adc = mcu::adc_01;
using sensor_pin = mcu::pa_00;
using sensor_channel = mcu::mux::adc_channel<sensor_adc, sensor_pin>;

using timer = mcu::timer_02;
using pin = mcu::pa_15;
using pwm_channel = mcu::mux::pwm_channel<timer, pin>;

using i2c_01 = mcu::i2c_01;
using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c_01, sizeof(void *) * 8>;
extern i2c_req_dispatcher_type i2c_dispatcher;
extern zoal::periph::i2c_request &request;
extern zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> io_events;

using w25q32 = zoal::ic::w25qxx<mcu::spi_01, mcu::pa_04, delay>;

auto constexpr buffer_mem_type = zoal::periph::device_buffer_type::shared_mem;
extern zoal::ic::ds3231<buffer_mem_type> clock;
extern zoal::ic::at24cxx<buffer_mem_type> eeprom;

void zoal_init_hardware();

#endif

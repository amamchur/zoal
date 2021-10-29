#ifndef ZOAL_STM32F401_FREERTOS_HARDWARE_HPP
#define ZOAL_STM32F401_FREERTOS_HARDWARE_HPP

#include "stm32f4xx_hal.h"

#include <zoal/freertos/event_group.hpp>
#include <zoal/freertos/mutex.hpp>
#include <zoal/freertos/scoped_lock.hpp>
#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/ic/at24cxx.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/w25qxx.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/mcu/stm32f401ccux.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/cmsis_os2/delay.hpp>

constexpr uint32_t system_clock_freq = 84000000;
constexpr uint32_t ahb_clock_freq = 84000000;
constexpr uint32_t apb1_clock_freq = 42000000;
constexpr uint32_t apb2_clock_freq = 84000000;

using mcu = zoal::mcu::stm32f401ccux;
using delay = zoal::utils::cmsis_os2::delay<system_clock_freq>;

using tty_usart = mcu::usart_01;
using tty_usart_rx = mcu::pb_07;
using tty_usart_tx = mcu::pb_06;

using sensor_adc = mcu::adc_01;
using sensor_pin = mcu::pa_00;
using sensor_channel = mcu::mux::adc_channel<sensor_adc, sensor_pin>;

using pwm_timer = mcu::timer_02;
using pwm_pin = mcu::pa_15;
using pwm_channel = mcu::mux::pwm_channel<pwm_timer, pwm_pin>;

using flash_spi = mcu::spi_01;
using flash_spi_mosi = mcu::pa_06;
using flash_spi_miso = mcu::pa_07;
using flash_spi_sck = mcu::pa_05;
using flash_spi_cs = mcu::pa_05;
using w25q32 = zoal::ic::w25qxx<flash_spi, flash_spi_cs, delay>;

using main_i2c = mcu::i2c_01;
using main_i2c_sda = mcu::pb_09;
using main_i2c_scl = mcu::pb_08;

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

using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<main_i2c, sizeof(void *) * 8>;
extern i2c_req_dispatcher_type i2c_dispatcher;
extern zoal::periph::i2c_request &request;
extern zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> io_events;

auto constexpr buffer_mem_type = zoal::periph::device_buffer_type::shared_mem;
extern zoal::ic::ds3231<buffer_mem_type> clock;
extern zoal::ic::at24cxx<buffer_mem_type> eeprom;

constexpr EventBits_t i2c_event = 1 << 0;
constexpr EventBits_t usart_event = 1 << 1;
constexpr EventBits_t all_hardware_events = i2c_event | usart_event;
extern zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> hardware_events;

void zoal_init_hardware();

#endif

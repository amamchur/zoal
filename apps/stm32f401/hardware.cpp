#include "hardware.hpp"

#include "stm32f4xx_hal.h"

#include <zoal/periph/i2c.hpp>
#include <zoal/utils/interrupts.hpp>

zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer(1);
zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer(1);

usart_01_tx_transport usart_tx;
usart_tx_stream_type tx_stream(usart_tx);
tx_stream_mutex_type tx_stream_mutex;

i2c_req_dispatcher_type i2c_dispatcher;
zoal::periph::i2c_request &request = i2c_dispatcher.request;

uint8_t i2c_shared_buffer[32];
zoal::ic::ds3231<buffer_mem_type> clock(i2c_shared_buffer);
zoal::ic::at24cxx<buffer_mem_type> eeprom(i2c_shared_buffer);

zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> hardware_events;

void zoal_init_hardware() {
    using api = zoal::gpio::api;
    using tty_usart_params = zoal::periph::usart_115200<apb2_clock_freq>;
    using tty_usart_mux = mcu::mux::usart<tty_usart, tty_usart_rx, tty_usart_tx>;
    using tty_usart_cfg = mcu::cfg::usart<tty_usart, tty_usart_params>;

    using flash_params = zoal::periph::spi_params<apb2_clock_freq>;
    using flash_spi_mux = mcu::mux::spi<flash_spi, flash_spi_mosi, flash_spi_miso, flash_spi_sck>;
    using flash_spi_cfg = mcu::cfg::spi<flash_spi, flash_params>;

    using oled_params = zoal::periph::spi_params<apb2_clock_freq>;
    using oled_spi_mux = mcu::mux::spi<oled_spi, oled_mosi, oled_miso, oled_sck>;
    using oled_spi_cfg = mcu::cfg::spi<oled_spi, oled_params>;

    using adc_params = zoal::periph::adc_params<>;
    using adc_cfg = mcu::cfg::adc<sensor_adc, adc_params>;

    using timer_params = zoal::periph::timer_params<apb2_clock_freq, 840, 1000, zoal::periph::timer_mode::up>;
    using timer_cfg = mcu::cfg::timer<mcu::timer_02, timer_params>;

    using i2c_params = zoal::periph::i2c_fast_mode<apb1_clock_freq>;
    using i2c_mux = mcu::mux::i2c<main_i2c, main_i2c_sda, main_i2c_scl>;
    using i2c_cfg = mcu::cfg::i2c<main_i2c, i2c_params>;

    api::optimize<
        //
        tty_usart_mux::clock_on,
        tty_usart_cfg::clock_on,
        flash_spi_mux::clock_on,
        flash_spi_cfg::clock_on,
        oled_spi_mux::clock_on,
        oled_spi_cfg::clock_on,
        i2c_mux::clock_on,
        i2c_cfg::clock_on,
        adc_cfg::clock_on,
        timer_cfg::clock_on
        //
        >();
    api::optimize<api::disable<tty_usart, flash_spi, sensor_adc, pwm_timer, main_i2c, oled_spi>>();

    api::optimize<
        //
        tty_usart_mux::connect,
        tty_usart_cfg::apply,
        //
        adc_cfg::apply,

        flash_spi_mux::connect,
        flash_spi_cfg::apply,
        //
        oled_spi_mux::connect,
        oled_spi_cfg::apply,

        i2c_mux::connect,
        i2c_cfg::apply,
        //
        timer_cfg::apply,
        //
        api::mode<zoal::gpio::pin_mode::output, flash_spi_cs, oled_cs, oled_ds, oled_res>,
        api::high<flash_spi_cs, oled_cs>
        //
        >();

    // Enable peripherals after configuration
    api::optimize<api::enable<tty_usart, flash_spi, sensor_adc, pwm_timer, main_i2c, oled_spi>>();

    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    tty_usart::enable_rx();

    zoal::utils::interrupts::on();
}

void usart_01_tx_transport::send_byte(uint8_t value) {
    tx_stream_buffer.send(value, portMAX_DELAY);
    tty_usart ::enable_tx();
}

void usart_01_tx_transport::send_data(const void *data, size_t size) {
    auto ptr = reinterpret_cast<const char *>(data);
    while (size > 0) {
        auto sent = tx_stream_buffer.send(ptr, size, 0);
        tty_usart ::enable_tx();
        size -= sent;
        ptr += sent;
    }
}

extern "C" void USART1_IRQHandler() {
    tty_usart::tx_handler([](uint8_t &value) {
        //
        return tx_stream_buffer.receive_isr(&value, 1) > 0;
    });
    tty_usart::rx_handler([](uint8_t byte) {
        rx_stream_buffer.send_isr(byte);
        hardware_events.set_isr(usart_event);
    });
}

extern "C" void I2C1_EV_IRQHandler() {
    mcu::i2c_01::handle_request_irq(request, []() {
        //
        hardware_events.set_isr(i2c_event);
    });
}

extern "C" void I2C1_ER_IRQHandler() {
    mcu::i2c_01::handle_request_irq(request, []() {
        //
        hardware_events.set_isr(i2c_event);
    });
}

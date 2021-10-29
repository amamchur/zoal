#include "hardware.hpp"

#include "stm32f1xx_hal.h"

#include <zoal/periph/i2c.hpp>
#include <zoal/utils/interrupts.hpp>

zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer(1);
zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer(1);

usart_01_tx_transport usart_tx;
usart_tx_stream_type tx_stream(usart_tx);
tx_stream_mutex_type tx_stream_mutex;

i2c_req_dispatcher_type i2c_dispatcher;

zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> hardware_events;

uint8_t i2c_shared_buffer[32];
zoal::ic::ds3231<buffer_mem_type> clock(i2c_shared_buffer);
zoal::ic::at24cxx<buffer_mem_type> eeprom(i2c_shared_buffer);
zoal::utils::i2c_scanner scanner;

constexpr uint32_t apb1_periph_clock_freq = 36000000;
constexpr uint32_t apb2_periph_clock_freq = 72000000;

void zoal_init_hardware() {
    using api = zoal::gpio::api;
    using i2c_02_cfg = zoal::periph::i2c_fast_mode<apb1_periph_clock_freq>;
    using i2c_mux = mcu::mux::i2c<i2c_02, mcu::pb_11, mcu::pb_10>;
    using i2c_cfg = mcu::cfg::i2c<i2c_02, i2c_02_cfg>;

    using usart_01_cfg = zoal::periph::usart_115200<apb2_periph_clock_freq>;
    using usart_mux = mcu::mux::usart<tty_usart, mcu::pb_07, mcu::pb_06>;
    using usart_cfg = mcu::cfg::usart<tty_usart, usart_01_cfg>;

    // Enable bus clock for peripherals
    api::optimize<
        //
        usart_mux::clock_on,
        usart_cfg::clock_on,
        //
        i2c_mux::clock_on,
        i2c_cfg::clock_on
        //
        >();

    // Disable peripherals before configuration
    api::optimize<api::disable<tty_usart, i2c_02>>();

    // Configuring everything
    api::optimize<
        //
        usart_mux::connect,
        usart_cfg::apply,
        //
        i2c_mux::connect,
        i2c_cfg::apply,
        //
        user_led::gpio_cfg,
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12, mcu::pb_13>>();

    // Enable peripherals after configuration
    api::optimize<api::enable<tty_usart, i2c_02>>();

    HAL_NVIC_SetPriority(USART1_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);

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
    tty_usart::tx_handler([](uint8_t &value) { return tx_stream_buffer.receive_isr(&value, 1) > 0; });
    tty_usart::rx_handler([](uint8_t byte) {
        rx_stream_buffer.send_isr(byte);
        hardware_events.set_isr(usart_event);
    });
}

extern "C" void I2C2_EV_IRQHandler(void) {
    i2c_02::handle_request_irq(i2c_dispatcher.request, []() { hardware_events.set_isr(i2c_event); });
}

extern "C" void I2C2_ER_IRQHandler(void) {
    i2c_02::handle_request_irq(i2c_dispatcher.request, []() { hardware_events.set_isr(i2c_event); });
}

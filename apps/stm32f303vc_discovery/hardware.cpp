#include "hardware.hpp"

#include "stm32f3xx_hal.h"

i2c_req_dispatcher_type i2c_dispatcher;
zoal::periph::i2c_request &request = i2c_dispatcher.request;

usart_debug_tx_transport usart_tx;
usart_tx_stream_type tx_stream(usart_tx);
tx_stream_mutex_type tx_stream_mutex;

zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> io_events;
zoal::ic::lsm303dlhc<> lsm303dlhc;

void zoal_init_hardware() {
    using api = zoal::gpio::api;
    using usart_speed_cfg = zoal::periph::usart_115200<36000000>;
    using usart_mux = mcu::mux::usart<tty_usart, mcu::pb_11, mcu::pb_10>;
    using usart_cfg = mcu::cfg::usart<tty_usart, usart_speed_cfg>;

    api::optimize<
        //
        usart_mux::periph_clock_on,
        usart_cfg::periph_clock_on
        //
        >();
    api::optimize<api::disable<tty_usart>>();

    // Configuring everything
    api::optimize<
        //
        usart_mux::connect,
        usart_cfg::apply,
        pcb::led_gpio_cfg
        //
        >();
    api::optimize<api::enable<tty_usart>>();

    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    tty_usart::enable_rx();
}

void usart_debug_tx_transport::send_byte(uint8_t value) {
    txs.send(value, portMAX_DELAY);
    tty_usart::enable_tx();
}

void usart_debug_tx_transport::send_data(const void *data, size_t size) {
    auto ptr = reinterpret_cast<const char *>(data);
    while (size > 0) {
        auto sent = txs.send(ptr, size, 0);
        tty_usart::enable_tx();
        size -= sent;
        ptr += sent;
    }
}

extern "C" void USART3_IRQHandler() {
    tty_usart::tx_handler([](uint8_t &value) { return usart_tx.txs.receive_isr(&value, 1) > 0; });
    tty_usart::rx_handler([](uint8_t byte) { usart_tx.rxs.send_isr(byte); });
}

extern "C" void I2C1_EV_IRQHandler() {
    i2c_01::handle_request_irq(request, []() { io_events.set_isr(1); });
}

extern "C" void I2C1_ER_IRQHandler() {
    i2c_01::handle_request_irq(request, []() { io_events.set_isr(1); });
}

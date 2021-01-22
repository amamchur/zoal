#include "templates/uno_lcd_shield.hpp"

#include <avr/io.h>
#include <zoal/arch/avr/atmega/i2c.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/shield/uno_lcd.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;

using timer = zoal::pcb::mcu::timer_00;
using counter_irq_handler = counter::handler<F_CPU, 64, timer>;
using usart = mcu::usart_01;

zoal::data::ring_buffer<uint8_t, 16> rx_buffer;
using usart_tx_transport = zoal::utils::usart_transmitter<usart, 16, zoal::utils::interrupts_off>;

using adc = mcu::adc_00;
using i2c = mcu::i2c_00;
using logger = zoal::utils::plain_logger<usart_tx_transport, zoal::utils::log_level::trace>;

scheduler_type scheduler;

void initialize_hardware() {
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;

    mcu::power<usart, timer, i2c, adc>::on();

    mcu::mux::usart<usart, zoal::pcb::ard_d00, zoal::pcb::ard_d01, mcu::pd_05>::connect();
    mcu::cfg::usart<usart, usart_cfg>::apply();

    mcu::mux::i2c<i2c, mcu::pd_01, mcu::pd_00>::connect();
    mcu::cfg::i2c<i2c, F_CPU>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::cfg::adc<adc>::apply();
    adc::enable_interrupt();

    mcu::enable<usart, timer, i2c, adc>::on();

    zoal::utils::interrupts::on();
}

int main() {
    initialize_hardware();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    logger::info() << "--- Start ---";

    while (true) {
        scheduler.handle();
    }

    return 0;
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    counter_irq_handler::increment();
}

ISR(USART1_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART1_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_transport::tx_buffer.pop_front(value); });
}

ISR(TWI_vect) {
    i2c::handle_irq();
}

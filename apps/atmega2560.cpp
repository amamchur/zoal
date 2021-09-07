#include <avr/eeprom.h>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_mega.hpp>
#include <zoal/data/ring_buffer.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using pcb = zoal::board::arduino_mega;
using mcu = pcb::mcu;
using timer = typename mcu::timer_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using delay = zoal::utils::delay<F_CPU, counter>;
;
using usart = mcu::usart_01;
using blink_pin = pcb::ard_d13;

zoal::data::ring_buffer<uint8_t, 16> rx_buffer;
using usart_tx_type = zoal::utils::usart_transmitter<usart, 16, zoal::utils::interrupts_off>;
using usart_tx_stream_type = zoal::io::output_stream<usart_tx_type>;

usart_tx_type usart_tx;
usart_tx_stream_type stream(usart_tx);

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_9600<F_CPU>;

    api::optimize<api::disable<usart, timer>>();
    api::optimize<
        //
//        mcu::mux::usart<usart, mcu::pe_00, mcu::pe_01>::connect,
//        mcu::cfg::usart<usart, usart_cfg>::apply,

        mcu::mux::usart<usart, mcu::pd_02, mcu::pd_03>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,

        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        mcu::irq::timer<timer>::enable_overflow_interrupt,

        api::mode<zoal::gpio::pin_mode::output, blink_pin>
        //
        >();

    api::optimize<api::enable<usart, timer>>();

    zoal::utils::interrupts::on();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
    using zoal::gpio::pin_mode;

    initialize_hardware();

    stream << "Hello\r\n";
    int step = 0;
    while (true) {
        blink_pin::_0();
        delay::ms(100);

        blink_pin::_1();
        delay::ms(100);

        stream << "Step: " << step++ << "\r\n";
    }
    return 0;
}
#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

ISR(USART1_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART1_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_type::tx_buffer.pop_front(value); });
}

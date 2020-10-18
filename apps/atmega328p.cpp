#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <zoal/arch/avr/stream.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using pcb = zoal::pcb;
using mcu = zoal::pcb::mcu;
using timer = mcu::timer_00;
using usart = mcu::usart_00;
using spi = mcu::spi_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using irq_handler = counter::handler<mcu::frequency, 64, timer>;

using tx_buffer = usart::default_tx_buffer<16>;
//using rx_buffer = usart::default_rx_buffer<16>;
using rx_buffer = usart::null_tx_buffer;

using logger = zoal::utils::terminal_logger<tx_buffer, zoal::utils::log_level::trace>;
//using logger = zoal::utils::plain_logger<tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = tools::delay;
using api = zoal::gpio::api;
using blink_pin = pcb::ard_d08;

void initialize_hardware() {
    // Power on modules
    api::optimize<api::power_on<usart, timer>>();

    // Disable all modules before applying settings
    api::optimize<api::disable<usart, timer>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect,
        mcu::mux::spi<spi, mcu::pb_03, mcu::pb_04, mcu::pb_05, mcu::pb_02>::connect,
        //
        mcu::cfg::usart<usart, 115200>::apply,
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        //
        //        api::power_off<mcu::port_b, mcu::port_c, mcu::port_d>,
        api::mode<zoal::gpio::pin_mode::output, blink_pin>,
        mcu::irq::timer<timer>::enable_overflow_interrupt
        //
        >();

    // Enable system interrupts
    zoal::utils::interrupts::on();

    // Enable all modules
    api::optimize<api::enable<usart, timer>>();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {
    initialize_hardware();

    logger::info() << "-- Start";

    while (true) {
        blink_pin::low();
        delay::ms(500);

        blink_pin::high();
        delay::ms(500);

        logger::info() << "-- Start";
    }
}

#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART_RX_vect) {
    usart::rx_handler<rx_buffer>();
}

ISR(USART_UDRE_vect) {
    usart::tx_handler<tx_buffer>();
}

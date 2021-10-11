#include "../../misc/terminal_input.hpp"
#include "command_machine.hpp"

#include <avr/eeprom.h>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_mega.hpp>
#include <zoal/ct/type_list.hpp>
#include <zoal/data/ring_buffer.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

FUSES = {.low = 0xFF, .high = 0xD7, .extended = 0xFC};

volatile uint32_t milliseconds = 0;

using pcb = zoal::board::arduino_mega;
using mcu = pcb::mcu;
using ms_timer = typename mcu::timer_00;
using pwm_timer = typename mcu::timer_04;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using delay = zoal::utils::delay<F_CPU, counter>;
using usart = mcu::usart_00;
using blink_pin = pcb::ard_d13;
using pwm_ch = mcu::mux::pwm_channel<pwm_timer, mcu::ph_05>;

using usart_tx_type = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
using usart_tx_stream_type = zoal::io::output_stream<usart_tx_type>;
char terminal_buffer[32];
auto terminal_greeting = "\033[0;32mroot@mcu\033[m$ ";
auto terminal = zoal::misc::terminal_input(terminal_buffer, sizeof(terminal_buffer));

zoal::data::ring_buffer<uint8_t, 32> rx_buffer;
usart_tx_type usart_tx;
usart_tx_stream_type stream(usart_tx);

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;
    api::optimize<api::clock_on<usart, ms_timer>>();
    api::optimize<api::disable<usart, ms_timer>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pe_00, mcu::pe_01>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,

        mcu::cfg::timer<ms_timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        mcu::irq::timer<ms_timer>::enable_overflow_interrupt,

        api::mode<zoal::gpio::pin_mode::output, blink_pin>
        //
        >();

    api::optimize<api::enable<usart, ms_timer>>();

    zoal::utils::interrupts::on();
}

class functor {
public:
    template<class T>
    void operator()() {
        stream << "A: " << zoal::io::hexadecimal(T::type::address);
        stream << " C: " << zoal::io::hexadecimal(T::type::clear);
        stream << " S: " << zoal::io::hexadecimal(T::type::set);
        stream << "\r\n";
    }
};

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx.send_data(s, e - s);
}

static void command_callback(zoal::misc::command_machine *, zoal::misc::command cmd, int argc, zoal::misc::cmd_arg *argv) {
    switch (cmd) {
    case zoal::misc::command::pwm:
        pwm_ch::set((int)*argv);
        break;
    case zoal::misc::command::pwm_connect:
        pwm_ch::connect();
        break;
    case zoal::misc::command::pwm_disconnect:
        pwm_ch::disconnect();
        break;
    default:
        break;
    }
}

static void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    stream << "\r\n";

    zoal::misc::command_machine cm;
    cm.callback(command_callback);
    cm.run_machine(s, e, e);
    terminal.sync();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    using zoal::gpio::pin_mode;

    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();
    terminal.sync();

    using cfg = mcu::cfg::timer<pwm_timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>;
    using list = zoal::gpio::api::optimize<cfg::apply, pwm_ch::connect>;
    list();

    while (true) {
        uint8_t rx_byte = 0;
        bool result;
        {
            zoal::utils::interrupts_off scope_off;
            result = rx_buffer.pop_front(rx_byte);
        }

        if (result) {
            terminal.push_and_scan(&rx_byte, 1);
        }
    }
    return 0;
}
#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

ISR(USART0_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART0_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_type::tx_buffer.pop_front(value); });
}

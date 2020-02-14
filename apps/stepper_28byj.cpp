#include "stepper_28byj_machine.hpp"

#include <avr/interrupt.h>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/io/stepper_28byj.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using pcb = zoal::pcb;
using mcu = pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using stepper_timer = mcu::timer_02;
using irq_handler = counter::handler<mcu::frequency, 64, timer>;
using usart = mcu::usart_00;
using tx_buffer = usart::default_tx_buffer<16>;
using rx_buffer = usart::default_rx_buffer<16>;
using logger = zoal::utils::plain_logger<tx_buffer, zoal::utils::log_level::trace, void, zoal::io::new_line_cr_lf>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = tools::delay;
using led_pin = pcb::ard_d13;


using stepper_type = zoal::io::stepper_28byj<tools, pcb::ard_d07, pcb::ard_d06, pcb::ard_d05, pcb::ard_d04, 4>;
stepper_type stepper;

static constexpr int MsgLength = 128;
using parser_type = stepper_28byj_parser<MsgLength>;
parser_type parser;

uint32_t cmd_steps = 0;
uint8_t cmd_direction = 0;

void (*command_handler)(parser_type *p, command_line_event event) = nullptr;
void start_handler(parser_type *p, command_line_event event);

int32_t token_to_int(parser_type *p) {
    int32_t value = 0;
    int sign = 1;
    auto s = p->token_start();
    auto e = p->token_end();
    for (; s != e; s++) {
        if (*s == '-') {
            sign -= 1;
        }

        value = value * 10 + (*s - '0');
    }

    return value * sign;
}

void callback(void *parser, int event) {
    auto p = reinterpret_cast<parser_type *>(parser);
    auto cle = static_cast<command_line_event>(event);
    command_handler(p, cle);
}

void skip_until_end_line(parser_type *p, command_line_event event) {
    switch (event) {
    case command_line_event::line_end:
        command_handler = &start_handler;
        break;
    default:
        break;
    }
}

void read_direction(parser_type *p, command_line_event event) {
    switch (event) {
    case command_line_event::number_token:
        cmd_direction = token_to_int(p);
        stepper.rotate(cmd_steps, cmd_direction);
        command_handler = &start_handler;
        break;
    default:
        command_handler = &skip_until_end_line;
        break;
    }
}

void read_steps(parser_type *p, command_line_event event) {
    switch (event) {
    case command_line_event::number_token:
        cmd_steps = token_to_int(p);
        command_handler = &read_direction;
        break;
    default:
        command_handler = &skip_until_end_line;
        break;
    }
}

void start_handler(parser_type *p, command_line_event event) {
    switch (event) {
    case command_line_event::command_rotate:
        command_handler = &read_steps;
        break;
    case command_line_event::command_stop:
        stepper.stop();
        break;
    default:
        break;
    }
}

int main() {
    mcu::power<timer, stepper_timer>::on();

    stepper.init();

    mcu::mux::usart<usart, pcb::ard_d00, pcb::ard_d01>::on();
    mcu::cfg::usart<usart, 115200>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::cfg::timer<stepper_timer, zoal::periph::timer_mode::up, 128, 1, 0xFF>::apply();
    mcu::irq::timer<stepper_timer>::enable_overflow_interrupt();

    mcu::enable<usart, timer, stepper_timer>::on();

    zoal::utils::interrupts::on();

    logger::info() << "Hello";

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    command_handler = &start_handler;
    parser.callback(&callback);

    while (true) {
        uint8_t rx_byte = 0;
        auto result = rx_buffer::pop_front(rx_byte);
        if (result) {
            parser.push(&rx_byte, 1);
            tx_buffer::push_back_blocking(rx_byte);
        }

        auto status = stepper.step_if_needed();
        switch (status) {
            case stepper_type::finished:
                logger::info() << "finished";
                break;
            default:
                break;
        }
    }
#pragma clang diagnostic pop
    return 0;
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(TIMER2_OVF_vect) {
    stepper.step_allowed();
}

ISR(USART_RX_vect) {
    usart::rx_handler_v2<rx_buffer>();
}

ISR(USART_UDRE_vect) {
    usart::tx_handler_v2<tx_buffer>();
}

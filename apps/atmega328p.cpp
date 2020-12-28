#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"

#include <avr/eeprom.h>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/vt100.hpp>
#include <zoal/arch/avr/stream.hpp>

volatile uint32_t milliseconds = 0;

using pcb = zoal::pcb;
using mcu = zoal::pcb::mcu;
using timer = mcu::timer_00;
using usart = mcu::usart_00;
using spi = mcu::spi_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using irq_handler = counter::handler<mcu::frequency, 64, timer>;

using tx_buffer = usart::default_tx_buffer<16>;
using rx_buffer = usart::default_rx_buffer<16>;
//using rx_buffer = usart::null_tx_buffer;

using logger = zoal::utils::terminal_logger<tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = tools::delay;
using api = zoal::gpio::api;
using blink_pin = pcb::ard_d08;
using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;

scheduler_type scheduler;
constexpr size_t max_input_str = 128;
using cmd_parser = zoal::misc::command_line_parser<max_input_str>;
char terminal_buffer[max_input_str];
zoal::misc::terminal_input term(terminal_buffer, sizeof(terminal_buffer));
auto greeting = "\033[0;32mmcu\033[m$ ";

const char help_msg[] PROGMEM = "\r\nMCU command list:"
                                "\r\n\tstart-blink"
                                "\r\n\tend-blink";
const char cmd_not_found_msg[] PROGMEM = "\r\nCommand not found";
const char help_cmd[] PROGMEM = "help";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";

void initialize_hardware() {
    // Power on modules
    api::optimize<api::power_on<usart, timer, spi>>();

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
        api::mode<zoal::gpio::pin_mode::output, blink_pin>,
        mcu::irq::timer<timer>::enable_overflow_interrupt
        //
        >();

    // Enable system interrupts
    zoal::utils::interrupts::on();

    // Enable all modules
    api::optimize<api::enable<usart, timer, spi>>();
}

void led_on(void *);

void led_off(void *);

void led_on(void *) {
    ::blink_pin::high();
    scheduler.schedule(500, led_off);
}

void led_off(void *) {
    ::blink_pin::low();
    scheduler.schedule(500, led_on);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void vt100_callback(const zoal::misc::terminal_input *t, const char *s, const char *) {
    for (auto ch = s; *ch; ch++) {
        tx_buffer::push_back_blocking(*ch);
    }
}

bool cmp_str_token(zoal::io::progmem_str_iter s1, const char *ss, const char *se) {
    while (*s1 && ss < se) {
        if (*s1 != *ss) {
            return false;
        }
        ++s1;
        ++ss;
    }
    return !*s1 && ss == se;
}

bool cmp_str_token(const char *s1, const char *ss, const char *se) {
    while (*s1 && ss < se) {
        if (*s1++ != *ss++) {
            return false;
        }
    }
    return !*s1 && ss == se;
}

void cmd_select_callback(void *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto parser = reinterpret_cast<cmd_parser *>(p);
    auto ts = parser->token_start();
    auto te = parser->token_end();

    if (cmp_str_token(zoal::io::progmem_str_iter(help_cmd), ts, te)) {
        auto stream = logger::stream();
        stream << zoal::io::progmem_str(help_msg);
        parser->callback(&cmd_parser::empty_callback);
        return;
    }

    if (cmp_str_token(zoal::io::progmem_str_iter(start_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
        scheduler.schedule(0, led_on);
        parser->callback(&cmd_parser::empty_callback);
        return;
    }

    if (cmp_str_token(zoal::io::progmem_str_iter(stop_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        parser->callback(&cmd_parser::empty_callback);
        return;
    }

    parser->callback(&cmd_parser::empty_callback);
    logger::warn() << zoal::io::progmem_str(cmd_not_found_msg);
}

void input_callback(const zoal::misc::terminal_input *t, const char *s, const char *e) {
    cmd_parser p;
    p.callback(cmd_select_callback);
    p.push(s, e - s, e);
}

int main() {
    initialize_hardware();

    term.vt100_callback(&vt100_callback);
    term.input_callback(&input_callback);
    term.greeting(greeting);
    term.clear();
    term.sync();

    while (true) {
        uint8_t rx_byte = 0;
        auto result = rx_buffer::pop_front(rx_byte);
        if (result) {
//            logger::info() << (int)rx_byte;
            term.push(&rx_byte, 1);
        }
        scheduler.handle();
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

#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"
#include "../misc/timer_freq_calculator.hpp"

#include <avr/io.h>
#include <zoal/arch/avr/stream.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/gfx/renderer.hpp>
#include <zoal/periph/i2c.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

FUSES = {.low = 0xFF, .high = 0xD7, .extended = 0xFC};

volatile uint32_t milliseconds = 0;

using pcb = zoal::board::arduino_uno;
using mcu = pcb::mcu;
using timer = mcu::timer_00;
using usart = mcu::usart_00;
using spi = mcu::spi_00;
using adc = mcu::adc_00;
using i2c = mcu::i2c_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using tools = zoal::utils::tool_set<mcu, F_CPU, counter, void>;
using delay = tools::delay;

zoal::data::ring_buffer<uint8_t, 16> rx_buffer;

using usart_tx_target = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
usart_tx_target tx_target;
using tx_stream_type = zoal::io::output_stream<usart_tx_target>;
tx_stream_type stream(tx_target);

zoal::utils::i2c_scanner scanner;
using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c, sizeof(void *) * 4>;
i2c_req_dispatcher_type i2c_req_dispatcher;
zoal::periph::i2c_request &request = i2c_req_dispatcher.request;

using api = zoal::gpio::api;
using blink_pin = pcb::ard_d13;
using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;

scheduler_type scheduler;
constexpr size_t terminal_str_size = 64;
using command_line_parser = zoal::misc::command_line_parser;
char terminal_buffer[terminal_str_size];
zoal::misc::terminal_input terminal(terminal_buffer, sizeof(terminal_buffer));
auto terminal_greeting = "\033[0;32mmcu\033[m$ ";

const char help_msg[] PROGMEM = "ZOAL ATmega328p Demo Application\r\n"
                                "Commands: \r\n"
                                "\t start-blink\tstart blinking\r\n"
                                "\t stop-blink\tstop blinking\r\n"
                                "\t adc\t\tread adc value\r\n";
const char help_cmd[] PROGMEM = "help";
const char adc_cmd[] PROGMEM = "adc";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;
    using i2c_cfg = zoal::periph::i2c_fast_mode<F_CPU>;
    using adc_cfg = zoal::periph::adc_config<>;
    using i2c_cfg = zoal::periph::i2c_fast_mode<F_CPU>;

    // Power on modules
    api::optimize<api::clock_on<usart, i2c, timer>>();

    // Disable all modules before applying settings
    api::optimize<api::disable<usart, timer, adc, i2c>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc, adc_cfg>::apply,
        //
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        //
        mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::connect,
        mcu::cfg::i2c<i2c, i2c_cfg>::apply,
        //
        mcu::irq::timer<timer>::enable_overflow_interrupt
        //
        >();

    // Enable system interrupts
    zoal::utils::interrupts::on();

    // Enable all modules
    api::optimize<api::enable<usart, timer, adc, i2c>>();

    adc::enable();
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

void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    tx_target.send_data(s, e - s);
}

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    p->callback(&command_line_parser::empty_callback);

    stream << "\r\n";

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(help_cmd), ts, te)) {
        stream << zoal::io::progmem_str(help_msg);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(adc_cmd), ts, te)) {
        auto v = adc::read();
        stream << "ADC: " << v << "\r\n";
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(start_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
        scheduler.schedule(0, led_on);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(stop_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
    }

    terminal.sync();
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    command_line_parser cmd_parser(nullptr, 0);
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.scan(s, e, e);
    terminal.sync();
}

const char zoal_ascii_logo[] PROGMEM = "  __________          _\r\n"
                                       " |___  / __ \\   /\\   | |\r\n"
                                       "    / / |  | | /  \\  | |\r\n"
                                       "   / /| |  | |/ /\\ \\ | |\r\n"
                                       "  / /_| |__| / ____ \\| |____\r\n"
                                       " /_____\\____/_/    \\_\\______|\r\n"
                                       "\r\n";

#include <zoal/ic/sh1106.hpp>

using sh1106_interface = zoal::ic::sh1106_interface_i2c<0x3C>;
using display_type = zoal::ic::sh1106<128, 64, sh1106_interface>;
display_type display;

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();
    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);

    scanner.device_found = [&](uint8_t addr) { stream << "Device: " << addr << "\r\n"; };
    scanner.scan(i2c_req_dispatcher)([&](int) {
        stream << "Done!"
               << "\r\n";
        terminal.sync();
    });
    i2c_req_dispatcher.handle_until_finished();

    display.init(i2c_req_dispatcher)([&](int){});
    i2c_req_dispatcher.handle_until_finished();

    using adapter = zoal::ic::sh1106_adapter_0<128, 64>;
    using graphics = zoal::gfx::renderer<uint8_t, adapter>;
    auto g = graphics::from_memory(display.buffer.canvas);
    g->clear(0x00);
    g->draw_circle(16, 16, 10, 1);
    display.display(i2c_req_dispatcher)([](int) { terminal.sync(); });

    while (true) {
        uint8_t rx_byte = 0;
        bool result;
        {
            zoal::utils::interrupts_off scope_off;
            result = rx_buffer.pop_front(rx_byte);
        }

        if (result) {
            terminal.push(&rx_byte, 1);
        }

        scheduler.handle();
        i2c_req_dispatcher.handle();
    }
}

#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

ISR(USART_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_target::tx_buffer.pop_front(value); });
}

ISR(TWI_vect) {
    i2c::handle_request_irq(request);
}

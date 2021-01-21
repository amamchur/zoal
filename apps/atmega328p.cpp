#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"

#include <avr/eeprom.h>
#include <avr/io.h>
#include <zoal/arch/avr/stream.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/ic/hd44780.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

FUSES = {
    .low = 0xFF,
    .high = 0xD7,
    .extended = 0xFC
};

volatile uint32_t milliseconds = 0;
volatile uint16_t adc_value = 0;
volatile bool process_adc = false;

using pcb = zoal::pcb;
using mcu = zoal::pcb::mcu;
using timer = mcu::timer_00;
using usart = mcu::usart_00;
using spi = mcu::spi_00;
using adc = mcu::adc_00;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using counter_irq_handler = counter::handler<F_CPU, 64, timer>;

zoal::data::ring_buffer_ext<uint8_t, 16> rx_buffer;

using usart_tx_transport = zoal::utils::usart_transmitter<usart, 16, zoal::utils::interrupts_off>;
usart_tx_transport transport;
using tx_stream_type = zoal::io::output_stream<usart_tx_transport>;
tx_stream_type stream(transport);

using tools = zoal::utils::tool_set<mcu, F_CPU, counter, void>;
using delay = tools::delay;
using api = zoal::gpio::api;
using blink_pin = pcb::ard_d08;
using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;

scheduler_type scheduler;
constexpr size_t terminal_str_size = 128;
using command_line_parser = zoal::misc::command_line_parser;
char terminal_buffer[terminal_str_size];
zoal::misc::terminal_input terminal(terminal_buffer, sizeof(terminal_buffer));
auto terminal_greeting = "\033[0;32mmcu\033[m$ ";

const char help_msg[] PROGMEM = "MCU command list:";
const char cmd_lookup[] PROGMEM = "\thelp\r\n\tstart-blink\r\n\tstop-blink";
const char help_cmd[] PROGMEM = "help";
const char lcd_cmd[] PROGMEM = "lcd";
const char adc_cmd[] PROGMEM = "adc";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";

using analog_keypad_type = zoal::io::analog_keypad<tools, 5>;
const analog_keypad_type::button_value_type buttons_adc_values[] __attribute__((section(".eeprom"))) = {0, 131, 308, 481, 722};
analog_keypad_type analog_keypad;

using interface_type = zoal::ic::hd44780_interface_4bit<delay, pcb::ard_d08, pcb::ard_d09, pcb::ard_d04, pcb::ard_d05, pcb::ard_d06, pcb::ard_d07>;
using address_selector = zoal::ic::hd44780_address_selector<16, 2>;
using lcd_type = zoal::ic::hd44780<interface_type, address_selector>;
lcd_type lcd;

void initialize_hardware() {
    // Power on modules
    api::optimize<api::clock_on<usart, timer>>();

    // Disable all modules before applying settings
    api::optimize<api::disable<usart, timer, adc>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect,
        mcu::cfg::usart<usart, zoal::periph::usart_115200<F_CPU>>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc>::apply,
        //
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        //
        lcd_type::gpio_cfg,
        api::mode<zoal::gpio::pin_mode::output, blink_pin>,
        mcu::irq::timer<timer>::enable_overflow_interrupt
        //        mcu::irq::adc<adc>::
        //
        >();

    // Enable system interrupts
    zoal::utils::interrupts::on();

    // Enable all modules
    api::optimize<api::enable<usart, timer, adc>>();

    adc::enable();
    adc::enable_interrupt();

    eeprom_read_block(analog_keypad.adc_values, buttons_adc_values, sizeof(analog_keypad.adc_values));

    lcd.init();
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
    usart_tx_transport::send_data(s, e - s);
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

void lcd_display(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);

    auto ts = p->token_start();
    auto te = p->token_end();
    lcd.clear();
    lcd.send_data(ts, te - ts);
    terminal.sync();
}

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();

    stream << "\r\n";

    if (cmp_str_token(zoal::io::progmem_str_iter(help_cmd), ts, te)) {
        stream << zoal::io::progmem_str(help_msg) << zoal::io::progmem_str(cmd_lookup);
        p->callback(&command_line_parser::empty_callback);
    }

    if (cmp_str_token(zoal::io::progmem_str_iter(lcd_cmd), ts, te)) {
        p->callback(&lcd_display);
        return;
    }

    if (cmp_str_token(zoal::io::progmem_str_iter(adc_cmd), ts, te)) {
        auto v = adc::read();
        stream << "ADC: " << v << "\r\n";
        p->callback(&command_line_parser::empty_callback);
    }

    if (cmp_str_token(zoal::io::progmem_str_iter(start_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
        scheduler.schedule(0, led_on);
        p->callback(&command_line_parser::empty_callback);
    }

    if (cmp_str_token(zoal::io::progmem_str_iter(stop_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
        p->callback(&command_line_parser::empty_callback);
    }

    p->callback(&command_line_parser::empty_callback);
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

void analog_keypad_handler(zoal::io::button_event e, int button) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    stream << "Buttons press: " << button << "\r\n";
}

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    stream << zoal::io::progmem_str(zoal_ascii_logo);
    terminal.sync();

    adc::start();
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

        if (process_adc) {
            analog_keypad.handle(analog_keypad_handler, adc_value);
            adc::start();
        }

        scheduler.handle();
    }
}

#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    counter_irq_handler::increment();
}

ISR(USART_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_transport::tx_buffer.pop_front(value); });
}

ISR(ADC_vect) {
    adc_value = adc::value();
    process_adc = true;
}

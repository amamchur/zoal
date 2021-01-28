#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"

#include <avr/eeprom.h>
#include <avr/io.h>
#include <zoal/arch/avr/stream.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/shield/uno_multi_functional.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

FUSES = {.low = 0xFF, .high = 0xD7, .extended = 0xFC};

volatile uint32_t milliseconds = 0;
volatile uint16_t adc_value = 0;
volatile bool process_adc = false;

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
using shield_type = zoal::shield::uno_multi_functional<pcb, typeof(milliseconds)>;
shield_type shield;

constexpr uint32_t ir_period_microseconds = 100;
using ir_receiver_type = zoal::io::ir_remote_receiver<pcb::ard_d02, ir_period_microseconds>;
ir_receiver_type ir_receiver;

zoal::data::ring_buffer<uint8_t, 16> rx_buffer;

using usart_tx_transport = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
usart_tx_transport transport;
using tx_stream_type = zoal::io::output_stream<usart_tx_transport>;
tx_stream_type stream(transport);

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
                                "\t stop-blink\tstop blinking\r\n";
const char help_cmd[] PROGMEM = "help";
const char adc_cmd[] PROGMEM = "adc";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";

void initialize_hardware() {
    // Power on modules
    api::optimize<api::clock_on<usart, timer>>();

    // Disable all modules before applying settings
    api::optimize<api::disable<usart, timer, adc, i2c>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect,
        mcu::cfg::usart<usart, zoal::periph::usart_115200<F_CPU>>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc, zoal::periph::adc_config<>>::apply,
        //
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        //
        mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::connect,
        mcu::cfg::i2c<i2c, F_CPU>::apply,
        //
        mcu::irq::timer<timer>::enable_overflow_interrupt,
        //
        shield_type::gpio_cfg,
        ir_receiver_type::gpio_cfg
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
    usart_tx_transport::send_data(s, e - s);
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
        shield.dec_to_segments(v);
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

uint16_t display_value = 0;

void button_handler(zoal::io::button_event e, uint8_t b) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    switch (b) {
    case 0:
        display_value++;
        break;
    case 1:
        display_value--;
        break;
    case 2:
        display_value = 0;
        break;
    default:
        return;
    }

    shield.dec_to_segments(display_value);
}

using test_pin = pcb::ard_d05;

void configure_timer_2() {
    test_pin::mode<zoal::gpio::pin_mode::output>();

    //    // 20000 Hz (16000000/((24+1)*32))
    //    OCR2A = 24;
    //    // CTC
    //    TCCR2A |= (1 << WGM21);
    //    // Prescaler 32
    //    TCCR2B |= (1 << CS21) | (1 << CS20);
    //    // Output Compare Match A Interrupt Enable
    //    TIMSK2 |= (1 << OCIE2A);
    //
    //    TCCR2A = 0;
    //    TCCR2B = 0;
    //    TCNT2 = 0;

    // 10000 Hz (16000000/((24+1)*64))
    OCR2A = 24;
    // CTC
    TCCR2A |= (1 << WGM21);
    // Prescaler 64
    TCCR2B |= (1 << CS22);
    // Output Compare Match A Interrupt Enable
    TIMSK2 |= (1 << OCIE2A);
}

uint32_t invert_bit_order(uint32_t v) {
    uint32_t result = 0;
    constexpr auto bits = static_cast<uint8_t>(sizeof(uint32_t) << 3);
    for (int i = 0; i < bits; i++) {
        result <<= 1;
        result |= v & 1;
        v >>= 1;
    }

    return result;
}

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    terminal.sync();

    delay::ms(10);
    ir_receiver.start();
    configure_timer_2();

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

        if (ir_receiver.processed()) {
            uint32_t value;
            {
                zoal::utils::interrupts_off scope_off;
                value = ir_receiver.result();
                ir_receiver.start();
            }

            stream << value << "\r\n";
        }

        scheduler.handle();
        shield.handle_buttons(counter::now(), button_handler);
        shield.dynamic_indication();
    }
}

#pragma clang diagnostic pop

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

ISR(TIMER2_COMPA_vect) {
    ir_receiver.handle();
}

ISR(USART_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_transport::tx_buffer.pop_front(value); });
}

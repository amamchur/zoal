#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"
#include "../misc/type_detector.hpp"
#include "./data/font_5x8.hpp"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <zoal/arch/avr/stream.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/gfx/monospace_glyph_render.hpp>
#include <zoal/gfx/renderer.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/lm75.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/parse/type_parser.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using pcb = zoal::pcb;
using usart = mcu::usart_01;
using i2c = mcu::i2c_00;
using timer = zoal::pcb::mcu::timer_00;
using blink_pin = zoal::pcb::ard_d13;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using counter_irq_handler = counter::handler<F_CPU, 64, timer>;
using delay = zoal::utils::delay<F_CPU, counter>;
using tools = zoal::utils::tool_set<mcu, F_CPU, counter, void>;

zoal::data::ring_buffer<uint8_t, 64> rx_buffer;
using usart_tx_transport = zoal::utils::usart_transmitter<usart, 64, zoal::utils::interrupts_off>;
using tx_stream_type = zoal::io::output_stream<usart_tx_transport>;

usart_tx_transport transport;
tx_stream_type stream(transport);

using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;
scheduler_type scheduler;

zoal::ic::lm75 temp_sensor;
zoal::ic::ds3231 clock;
zoal::periph::i2c_request request;
using scanner_type = zoal::utils::i2c_scanner;
scanner_type scanner;

using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<tools, i2c, typename pcb::ard_d07, typename pcb::ard_d08, 0x3C>;
using ssd1306_type = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;

ssd1306_type display(request);

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;

    api::optimize<api::disable<usart, timer, i2c>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_02, mcu::pd_03>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,

        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        mcu::irq::timer<timer>::enable_overflow_interrupt,
        //
        mcu::mux::i2c<i2c, mcu::pd_01, mcu::pd_00>::connect,
        mcu::cfg::i2c<i2c, F_CPU>::apply,
        //
        api::mode<zoal::gpio::pin_mode::output, blink_pin>,
        ssd1306_interface::gpio_cfg
        //
        >();

    api::optimize<api::enable<usart, timer, i2c>>();

    zoal::utils::interrupts::on();
}

void print_temp() {
    stream << temp_sensor.temperature() << " C\r\n";
}

void print_date_time() {
    stream << clock.date_time() << "\r\n";
}

void update_date_time() {
    stream << "Data time updated\r\n";
}

void led_on(void *);
void led_off(void *);
void i2c_request_loop();

void led_on(void *) {
    ::blink_pin::high();
    scheduler.schedule(500, led_off);
}

void led_off(void *) {
    ::blink_pin::low();
    scheduler.schedule(500, led_on);
}

void request_temp(void *) {
    if (request.processing()) {
        return;
    }

    temp_sensor.fetch(request);
    request.context = reinterpret_cast<void *>(&print_temp);
    i2c::start();
}

void request_time(void *) {
    if (request.processing()) {
        return;
    }

    clock.fetch(request);
    request.context = reinterpret_cast<void *>(&print_date_time);
    i2c::start();
}

void scan_i2c() {
    if (request.processing()) {
        stream << "request_time request.processing: " << (int)request.result << "\r\n";
        return;
    }

    scanner.scan(request);
    i2c::start();
}

using command_line_parser = zoal::misc::command_line_parser;
char terminal_buffer[128];
auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
auto terminal = zoal::misc::terminal_input(terminal_buffer, sizeof(terminal_buffer));
const char zoal_ascii_logo[] PROGMEM = "  __________          _\r\n"
                                       " |___  / __ \\   /\\   | |\r\n"
                                       "    / / |  | | /  \\  | |\r\n"
                                       "   / /| |  | |/ /\\ \\ | |\r\n"
                                       "  / /_| |__| / ____ \\| |____\r\n"
                                       " /_____\\____/_/    \\_\\______|\r\n"
                                       "\r\n";
const char help_msg[] PROGMEM = "ZOAL ATmega32u4 Demo Application\r\n"
                                "Commands: \r\n"
                                "\tstart-blink\tstart blinking\r\n"
                                "\tstop-blink\tstop blinking\r\n"
                                "\ttemp\t\tdisplay temperature\r\n"
                                "\ttime\t\tdisplay date time\r\n"
                                "\tset-time\tset date time\r\n"
                                "\ti2c-scan\tscan i2c devises\r\n"
                                "\toled\tscan i2c devises\r\n";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";
const char temp_cmd[] PROGMEM = "temp";
const char time_cmd[] PROGMEM = "time";
const char set_time_cmd[] PROGMEM = "set-time";
const char i2c_scan_cmd[] PROGMEM = "i2c-scan";
const char oled_render_cmd[] PROGMEM = "oled";

void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx_transport::send_data(s, e - s);
}

template<size_t Width = 5, size_t Height = 8>
class progmem_bitmap_font {
public:
    using self_type = progmem_bitmap_font<Width, Height>;

    static constexpr size_t width = Width;
    static constexpr size_t height = Height;

    static const self_type *from_memory(const void *buffer) {
        return reinterpret_cast<const self_type *>(buffer);
    }

    uint16_t glyph_columns(char c, uint8_t index) const {
        return pgm_read_byte(data + c * width + index);
    }

    uint8_t data[0];
};

using adapter = zoal::ic::ssd1306_adapter_0<128, 64>;
using graphics = zoal::gfx::renderer<uint8_t, adapter>;
ssd1306_type::buffer_type ssd1306_buffer;

static void oled_render(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    stream << "OLED render\r\n";

    auto f = progmem_bitmap_font<>::from_memory(font_5x8);
    auto g = graphics::from_memory(ssd1306_buffer.canvas);
    zoal::gfx::monospace_glyph_render<graphics, progmem_bitmap_font<>> tl(g, f);
    g->clear(0);
    tl.position(0, 8).draw(p->token_start(), p->token_end(), 1);
    display.display_sync(ssd1306_buffer);
}

static void parser_time(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    zoal::misc::type_detector_v2 td;
    auto pos = td.detect(p->token_start(), p->token_end(), p->token_end());
    if (td.type() != zoal::misc::value_type::date_time) {
        return;
    }

    auto dt = zoal::parse::type_parser<zoal::data::date_time>::parse(pos);
    stream << "Parsed: " << dt << "\r\n";

    clock.date_time(dt);
    clock.update(request);
    request.context = reinterpret_cast<void *>(&update_date_time);
    i2c::start();
}

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    p->callback(&command_line_parser::empty_callback);

    stream << "\r\n";

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(start_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
        scheduler.schedule(0, led_on);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(stop_blink_cmd), ts, te)) {
        scheduler.clear_handle(led_on);
        scheduler.clear_handle(led_off);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(temp_cmd), ts, te)) {
        scheduler.schedule(0, request_temp);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(time_cmd), ts, te)) {
        scheduler.schedule(0, request_time);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(set_time_cmd), ts, te)) {
        p->callback(&parser_time);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(i2c_scan_cmd), ts, te)) {
        scan_i2c();
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(oled_render_cmd), ts, te)) {
        p->callback(&oled_render);
    }

    terminal.sync();
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    command_line_parser cmd_parser(nullptr, 0);
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.scan(s, e, e);
    terminal.sync();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    terminal.sync();

    display.init();

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

        i2c_request_loop();
        scheduler.handle();
    }

    return 0;
}
#pragma clang diagnostic pop

void i2c_request_loop() {
    using zoal::periph::i2c_request_completion_result;

    if (!request.finished()) {
        return;
    }

    auto result = temp_sensor.complete_request(request);
    if (result == i2c_request_completion_result::ignored) {
        result = clock.complete_request(request);
    }

    if (result == i2c_request_completion_result ::ignored) {
        result = scanner.complete_request(request, [](uintptr_t addr) {
            //
            stream << "I2C device found at address: " << reinterpret_cast<void *>(addr) << "\r\n";
        });
    }

    switch (result) {
    case i2c_request_completion_result::ignored:
        request.reset();
        break;
    case i2c_request_completion_result::finished:
        if (request.context != nullptr) {
            reinterpret_cast<void (*)()>(request.context)();
        }
        request.reset();
        terminal.sync();
        break;
    case i2c_request_completion_result::new_request:
        i2c::start();
        break;
    default:
        request.reset();
        terminal.sync();
        break;
    }
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
    i2c::handle_request_irq(request);
}

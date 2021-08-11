#include "../misc/cmd_argument.hpp"
#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"
#include "./data/font_5x8.hpp"
#include "./data/roboto_regular_16.hpp"

#include <zoal/arch/avr/stream.hpp>
#include <zoal/arch/avr/utils/progmem_reader.hpp>
#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/gfx/glyph_renderer.hpp>
#include <zoal/gfx/renderer.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/lm75.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/io/button.hpp>
#include <zoal/parse/type_parser.hpp>
#include <zoal/periph/i2c.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/shield/uno_accessory.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using pcb = zoal::board::arduino_leonardo;
using mcu = pcb::mcu;
using usart = mcu::usart_01;
using adc = mcu::adc_00;
using i2c = mcu::i2c_00;
using spi = mcu::spi_00;
using timer = pcb::mcu::timer_00;

using blink_pin = pcb::ard_d13;

using ms_counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using delay = zoal::utils::delay<F_CPU, ms_counter>;
using tools = zoal::utils::tool_set<mcu, F_CPU, ms_counter, void>;

using usart_tx_target = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
using tx_stream_type = zoal::io::output_stream<usart_tx_target>;
using command_line_parser = zoal::misc::command_line_parser;
char terminal_buffer[32];
auto terminal_greeting = "\033[0;32mroot@mcu\033[m$ ";
auto terminal = zoal::misc::terminal_input(terminal_buffer, sizeof(terminal_buffer));

zoal::data::ring_buffer<uint8_t, 32> rx_buffer;
usart_tx_target tx_target;
tx_stream_type stream(tx_target);

using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c, sizeof(void *) * 4>;
using shield = zoal::shield::uno_accessory<delay, pcb>;
using buzzer = shield::buzzer;
using adapter = zoal::ic::ssd1306_adapter_0<128, 64>;
using graphics = zoal::gfx::renderer<uint8_t, adapter>;
shield::display_type display;
shield::thermometer_type temp_sensor;
shield::clock_type clock;
shield::accelerometer_type accelerometer;

zoal::utils::i2c_scanner scanner;
i2c_req_dispatcher_type i2c_req_dispatcher;
zoal::periph::i2c_request &request = i2c_req_dispatcher.request;

using scheduler_type = zoal::utils::lambda_scheduler<uint32_t, 4, 8, uint8_t>;
scheduler_type scheduler;

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;
    using i2c_cfg = zoal::periph::i2c_fast_mode<F_CPU>;

    api::optimize<api::disable<usart, timer, i2c, spi, adc>>();
    api::optimize<
        //
        mcu::mux::usart<usart, mcu::pd_02, mcu::pd_03>::connect,
        mcu::cfg::usart<usart, usart_cfg>::apply,
        //
        mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply,
        mcu::irq::timer<timer>::enable_overflow_interrupt,
        //
        mcu::mux::i2c<i2c, mcu::pd_01, mcu::pd_00>::connect,
        mcu::cfg::i2c<i2c, i2c_cfg>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc, zoal::periph::adc_config<>>::apply,
        //
        api::mode<zoal::gpio::pin_mode::output, blink_pin>,
        api::mode<zoal::gpio::pin_mode::input_pull_up, pcb::ard_a01, pcb::ard_a02, pcb::ard_a03, pcb::ard_a04, pcb::ard_a05>,
        shield::gpio_cfg
        //
        >();

    api::optimize<api::enable<usart, timer, i2c, spi, adc>>();

    zoal::utils::interrupts::on();
}

void led_on();
void led_off();
constexpr scheduler_type::id_type led_on_id = 1;
constexpr scheduler_type::id_type led_off_id = 2;

void led_on() {
    ::blink_pin::high();
    scheduler.schedule(500, led_off, led_off_id);
}

void led_off() {
    ::blink_pin::low();
    scheduler.schedule(500, led_on, led_on_id);
}

void request_temp() {
    temp_sensor.fetch(i2c_req_dispatcher)([&](int) {
        stream << "\033[2K\r" << temp_sensor.temperature() << " C"
               << "\r\n";
        terminal.sync();
    });
}

void request_time() {
    clock.fetch(i2c_req_dispatcher)([&](int) {
        stream << "\033[2K\r" << clock.date_time() << "\r\n";
        terminal.sync();
    });
}

void scan_i2c() {
    scanner.device_found = [](uint8_t addr) {
        stream << "\033[2K\r"
               << "i2c device: " << addr << "\r\n";
    };
    scanner.scan(i2c_req_dispatcher)([](int) { terminal.sync(); });
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"

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
                                "\toled\t\tprint text on OLED screen\r\n"
                                "\tadc\t\tread adc value\r\n"
                                "\trgb\t\trgb\r\n"
                                "\trgb-off\t\trgb off\r\n";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";
const char temp_cmd[] PROGMEM = "temp";
const char time_cmd[] PROGMEM = "time";
const char set_time_cmd[] PROGMEM = "set-time";
const char i2c_scan_cmd[] PROGMEM = "i2c-scan";
const char oled_cmd[] PROGMEM = "oled";
const char axis_cmd[] PROGMEM = "axis";
const char adc_cmd[] PROGMEM = "adc";
const char rgb_cmd[] PROGMEM = "rgb";
const char rgb_off_cmd[] PROGMEM = "rgb-off";
const char help_cmd[] PROGMEM = "help";

#pragma clang diagnostic pop

void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    tx_target.send_data(s, e - s);
}

static void oled_render(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto g = graphics::from_memory(display.buffer.canvas);
    zoal::gfx::glyph_renderer<graphics, zoal::utils::progmem_reader> gl(g, &roboto_regular_16);
    g->clear(0);
    gl.color(1);
    gl.position(0, roboto_regular_16.y_advance);
    gl.draw(p->token_start(), p->token_end());
    display.display(i2c_req_dispatcher)([](int) { terminal.sync(); });
}

void read_axis() {
    accelerometer.fetch_axis(i2c_req_dispatcher)([](int) {
        auto axis = accelerometer.vector();
        stream << "\033[2K\r";
        stream << "x: " << axis.x << "\r\n";
        stream << "y: " << axis.y << "\r\n";
        stream << "z: " << axis.z << "\r\n";

        terminal.sync();
    });
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
    clock.date_time(dt);
    clock.update(i2c_req_dispatcher)([](int) { terminal.sync(); });
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
        scheduler.remove(led_on_id);
        scheduler.remove(led_off_id);
        scheduler.schedule(0, led_on);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(stop_blink_cmd), ts, te)) {
        scheduler.remove(led_on_id);
        scheduler.remove(led_off_id);
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

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(oled_cmd), ts, te)) {
        p->callback(&oled_render);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(axis_cmd), ts, te)) {
        read_axis();
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(adc_cmd), ts, te)) {
        auto v = adc::read();
        stream << "\033[2K\r" << v << "\r\n";
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(rgb_cmd), ts, te)) {
        shield::rgb(0, 0, 1);
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(rgb_off_cmd), ts, te)) {
        shield::rgb_off();
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(help_cmd), ts, te)) {
        stream << "\033[2K\r";
        stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    }

    terminal.sync();
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    command_line_parser cmd_parser(nullptr, 0);
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.scan(s, e, e);
    terminal.sync();
}

void accelerometer_initialized(int) {
    stream << "\033[2K\r";
    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    terminal.sync();
}

void display_initialized(int) {
    accelerometer.power_on(i2c_req_dispatcher)(accelerometer_initialized);
}

void initialize_i2c_devices() {
    shield::ssd1306_slave_address_setup();

    display.init(i2c_req_dispatcher)(display_initialized);
    i2c_req_dispatcher.handle_until_finished();
}

shield::joystick joystick;

auto button_handler = [](zoal::io::button_event e, shield::joystick_button b) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    switch (b) {
    case shield::joystick_button::right:
        terminal.cursor(terminal.cursor() + 1);
        break;
    case shield::joystick_button::left:
        terminal.cursor(terminal.cursor() - 1);
        break;
    case shield::joystick_button::enter:
        rx_buffer.push_back(13);
        break;
    default:
        break;
    }

    terminal.sync();
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

void fetch_axis();

void display_axis(const zoal::data::vector<int16_t>& v) {
    using gl_type = zoal::gfx::glyph_renderer<graphics, zoal::utils::progmem_reader>;
    auto g = graphics::from_memory(display.buffer.canvas);
    gl_type gl(g, &roboto_regular_16);
    zoal::io::output_stream<gl_type> os(gl);
    g->clear(0);
    gl.color(1);

    auto gvect = v.convert<float>() / 256;

    for (int i = 0; i < 3; i++) {
        gl.position(0, roboto_regular_16.y_advance * (i + 1));
        os << gvect[i];
    }

    display.display(i2c_req_dispatcher)([](int) { scheduler.schedule(20, fetch_axis); });
}

void fetch_axis() {
    accelerometer.fetch_axis(i2c_req_dispatcher)([](int) { display_axis(accelerometer.vector()); });
}

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    initialize_i2c_devices();

    buzzer::on();
    delay::ms(20);
    buzzer::off();

    auto g = graphics::from_memory(display.buffer.canvas);
    g->clear(0);
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

        i2c_req_dispatcher.handle();
        scheduler.handle(milliseconds);

        auto time = ms_counter::now();
        joystick.handle(time, button_handler);
    }

    return 0;
}
#pragma clang diagnostic pop

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"

ISR(TIMER0_OVF_vect) {
    milliseconds += overflow_to_tick::step();
}

ISR(USART1_RX_vect) {
    usart::rx_handler<>([](uint8_t value) { rx_buffer.push_back(value); });
}

ISR(USART1_UDRE_vect) {
    usart::tx_handler([](uint8_t &value) { return usart_tx_target::tx_buffer.pop_front(value); });
}

ISR(TWI_vect) {
    i2c::handle_request_irq(request);
}

#pragma clang diagnostic pop

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
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/lm75.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/io/button.hpp>
#include <zoal/parse/type_parser.hpp>
#include <zoal/periph/adc.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/periph/spi.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/new.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/vt100.hpp>

using namespace zoal::utils::vt100;

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using pcb = zoal::pcb;
using usart = mcu::usart_01;
using adc = mcu::adc_00;
using i2c = mcu::i2c_00;
using spi = mcu::spi_00;
using timer = zoal::pcb::mcu::timer_00;

using blink_pin = zoal::pcb::ard_d13;
using ms_counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using overflow_to_tick = zoal::utils::timer_overflow_to_tick<F_CPU, 32, 256>;
using delay = zoal::utils::delay<F_CPU, ms_counter>;
using tools = zoal::utils::tool_set<mcu, F_CPU, ms_counter, void>;

using usart_tx_transport = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
using tx_stream_type = zoal::io::output_stream<usart_tx_transport>;

usart_tx_transport transport;
using scheduler_type = zoal::utils::function_scheduler<ms_counter, 8, void *>;
using command_line_parser = zoal::misc::command_line_parser;
char terminal_buffer[32];
auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
auto terminal = zoal::misc::terminal_input(terminal_buffer, sizeof(terminal_buffer));

using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c, sizeof(void *) * 4>;
using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<delay, typename pcb::ard_d07, typename pcb::ard_d08, 0x3C>;
using ssd1306_type = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;

zoal::data::ring_buffer<uint8_t, 32> rx_buffer;
tx_stream_type stream(transport);

ssd1306_type display;
zoal::ic::lm75 temp_sensor;
zoal::ic::ds3231 clock;
zoal::ic::adxl345 accelerometer;
zoal::utils::i2c_scanner scanner;
i2c_req_dispatcher_type i2c_req_dispatcher;
zoal::periph::i2c_request &request = i2c_req_dispatcher.request;

scheduler_type scheduler;

zoal::io::button<uint32_t, typename pcb::ard_a01> u_button;
zoal::io::button<uint32_t, typename pcb::ard_a02> r_button;
zoal::io::button<uint32_t, typename pcb::ard_a03> l_button;
zoal::io::button<uint32_t, typename pcb::ard_a04> e_button;
zoal::io::button<uint32_t, typename pcb::ard_a05> d_button;

void initialize_hardware() {
    using namespace zoal::gpio;
    using usart_cfg = zoal::periph::usart_115200<F_CPU>;

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
        mcu::cfg::i2c<i2c, F_CPU>::apply,
        //
        mcu::mux::adc<adc, pcb::ard_a00>::connect,
        mcu::cfg::adc<adc, zoal::periph::adc_config<>>::apply,
        //
        api::mode<zoal::gpio::pin_mode::output, blink_pin>,
        ssd1306_interface::gpio_cfg,
        api::mode<zoal::gpio::pin_mode::input_pull_up, pcb::ard_a01, pcb::ard_a02, pcb::ard_a03, pcb::ard_a04, pcb::ard_a05>
        //
        >();

    api::optimize<api::enable<usart, timer, i2c, spi, adc>>();

    zoal::utils::interrupts::on();
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

void request_temp(void *) {
    temp_sensor.fetch(i2c_req_dispatcher)([&]() {
        stream << "\033[2K\r" << temp_sensor.temperature() << " ℃\r\n";
        terminal.sync();
    });
}

void request_time(void *) {
    clock.fetch(i2c_req_dispatcher)([&]() {
        stream << "\033[2K\r" << clock.date_time() << "\r\n";
        terminal.sync();
    });
}

void scan_i2c() {
    scanner.device_found = [](uint8_t addr) {
        stream << "\033[2K\r"
               << "i2c device: " << addr << "\r\n";
    };
    scanner.scan(i2c_req_dispatcher)([]() { terminal.sync(); });
}

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
                                "\toled\t\tscan i2c devises\r\n"
                                "\tadc\t\tread adc value\r\n";
const char start_blink_cmd[] PROGMEM = "start-blink";
const char stop_blink_cmd[] PROGMEM = "stop-blink";
const char temp_cmd[] PROGMEM = "temp";
const char time_cmd[] PROGMEM = "time";
const char set_time_cmd[] PROGMEM = "set-time";
const char i2c_scan_cmd[] PROGMEM = "i2c-scan";
const char oled_render_cmd[] PROGMEM = "oled";
const char axis_cmd[] PROGMEM = "axis";
const char adc_cmd[] PROGMEM = "adc";

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

static void oled_render(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    using adapter = zoal::ic::ssd1306_adapter_0<128, 64>;
    using graphics = zoal::gfx::renderer<uint8_t, adapter>;
    auto f = progmem_bitmap_font<>::from_memory(font_5x8);
    auto g = graphics::from_memory(display.buffer.canvas);
    zoal::gfx::monospace_glyph_render<graphics, progmem_bitmap_font<>> tl(g, f);
    g->clear(0);
    tl.position(0, 8).draw(p->token_start(), p->token_end(), 1);
    display.display(i2c_req_dispatcher)([]() { terminal.sync(); });
}

void read_axis() {
    accelerometer.fecth_axis(i2c_req_dispatcher)([]() {
        auto axis = accelerometer.raw_vector;
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
    clock.update(i2c_req_dispatcher)([]() { terminal.sync(); });
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

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(axis_cmd), ts, te)) {
        read_axis();
    }

    if (cmp_progmem_str_token(zoal::io::progmem_str_iter(adc_cmd), ts, te)) {
        auto v = adc::read();
        stream << "\033[2K\r" << v << "\r\n";
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

enum class joystick_button { up, right, button, left, enter };
auto button_handler = [](zoal::io::button_event e, joystick_button b) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    switch (b) {
    case joystick_button::right:
        terminal.cursor(terminal.cursor() + 1);
        break;
    case joystick_button::left:
        terminal.cursor(terminal.cursor() - 1);
        break;
    case joystick_button::enter:
        rx_buffer.push_back(13);
        break;
    default:
        break;
    }

    terminal.sync();
};

void accelerometer_initialized() {
    stream << "\033[2K\r";
    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    terminal.sync();
}

void display_initialized() {
    accelerometer.power_on(i2c_req_dispatcher)(accelerometer_initialized);
}

void initialize_i2c_devices() {
    bool ssd1306_found = false;
    bool lm75_found = true;
    bool ds3231_found = false;
    bool adxl345_found = false;

    display.assign_addr();

    scanner.device_found = [&](uint8_t addr) {
        ssd1306_found = ssd1306_found || addr == 0x3C;
        lm75_found = lm75_found || addr == 0x48;
        ds3231_found = ds3231_found || addr == 0x68;
        adxl345_found = adxl345_found || addr == 0x53;
    };
    scanner.scan(i2c_req_dispatcher)([&]() { terminal.sync(); });
    i2c_req_dispatcher.handle_until_finished();

    display.init(i2c_req_dispatcher)(display_initialized);
    i2c_req_dispatcher.handle_until_finished();
}

int main() {
    initialize_hardware();

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    initialize_i2c_devices();

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
        scheduler.handle();

        auto time = ms_counter::now();
        u_button.handle(time, button_handler, joystick_button::up);
        r_button.handle(time, button_handler, joystick_button::right);
        l_button.handle(time, button_handler, joystick_button::left);
        e_button.handle(time, button_handler, joystick_button::enter);
        d_button.handle(time, button_handler, joystick_button::button);
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
    usart::tx_handler([](uint8_t &value) { return usart_tx_transport::tx_buffer.pop_front(value); });
}

ISR(TWI_vect) {
    i2c::handle_request_irq(request);
}

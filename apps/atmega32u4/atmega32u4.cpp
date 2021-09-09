#include "../../misc/cmd_argument.hpp"
#include "../../misc/terminal_input.hpp"
#include "../data/roboto_regular_16.hpp"
#include "command_machine.hpp"

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

using usart_tx_type = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
using usart_tx_stream_type = zoal::io::output_stream<usart_tx_type>;
char terminal_buffer[32];
auto terminal_greeting = "\033[0;32mroot@mcu\033[m$ ";
auto terminal = zoal::misc::terminal_input(terminal_buffer, sizeof(terminal_buffer));

zoal::data::ring_buffer<uint8_t, 32> rx_buffer;
usart_tx_type usart_tx;
usart_tx_stream_type stream(usart_tx);

using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c, sizeof(void *) * 4>;
using shield = zoal::shield::uno_accessory<delay, pcb>;
using buzzer = shield::buzzer;
using adapter = zoal::ic::ssd1306_adapter_0<128, 64>;
using graphics = zoal::gfx::renderer<uint8_t, adapter>;
shield::display_type oled;
shield::thermometer_type temp_sensor;
shield::clock_type clock;
shield::accelerometer_type accelerometer;

zoal::utils::i2c_scanner scanner;
i2c_req_dispatcher_type i2c_dispatcher;

using scheduler_type = zoal::utils::function_scheduler<uint32_t, 4, uint8_t>;
scheduler_type scheduler;

constexpr scheduler_type::id_type toggle_led_id = 1;

static void initialize_hardware() {
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

static void toggle_led(uint8_t v) {
    ::blink_pin::write(v);
    scheduler.schedule(toggle_led_id, 500, toggle_led, 1 - v);
}

static void request_temp() {
    temp_sensor.fetch(i2c_dispatcher)([&](int) {
        stream << "\033[2K\r" << temp_sensor.temperature() << " C"
               << "\r\n";
        terminal.sync();
    });
}

static void request_time() {
    clock.fetch(i2c_dispatcher)([&](int) {
        stream << "\033[2K\r" << clock.date_time() << "\r\n";
        terminal.sync();
    });
}

static void scan_i2c() {
    scanner.device_found = [](uint8_t addr) {
        stream << "\033[2K\r"
               << "i2c device: " << addr << "\r\n";
    };
    scanner.scan(i2c_dispatcher)([](int) { terminal.sync(); });
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
                                "\tadc\t\t\t\tread adc value\r\n"
                                "\taxis\t\t\t\tread accelerometer\r\n"
                                "\tblink 0 or 1\t\t\tstart/stop blinking\r\n"
                                "\thelp\t\t\t\tdisplay help\r\n"
                                "\ti2c\t\t\t\tscan i2c devises\r\n"
                                "\toled \"msg\"\t\t\tprint text on OLED screen\r\n"
                                "\trgb r g b\t\t\tset rgb LED color\r\n"
                                "\trgb off\t\t\t\trgb off\r\n"
                                "\ttemp\t\t\t\tdisplay temperature\r\n"
                                "\ttime\t\t\t\tdisplay date time\r\n"
                                "\ttime yyyy.MM.dd HH:mm::ss\tset date time\r\n";

#pragma clang diagnostic pop

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx.send_data(s, e - s);
}

static void oled_render(const char *s, const char *e) {
    auto g = graphics::from_memory(oled.buffer.canvas);
    zoal::gfx::glyph_renderer<graphics, zoal::utils::progmem_reader> gl(g, &roboto_regular_16);
    g->clear(0);
    gl.color(1);
    gl.position(0, roboto_regular_16.y_advance);
    gl.draw(s, e);
    oled.display(i2c_dispatcher)([](int) { terminal.sync(); });
}

static void read_axis() {
    accelerometer.fetch_axis(i2c_dispatcher)([](int) {
        auto axis = accelerometer.vector();
        stream << "\033[2K\r";
        stream << "x: " << axis.x << "\r\n";
        stream << "y: " << axis.y << "\r\n";
        stream << "z: " << axis.z << "\r\n";

        terminal.sync();
    });
}

static void command_callback(zoal::misc::command_machine *, zoal::misc::command cmd, int argc, zoal::misc::cmd_arg *argv) {
    switch (cmd) {
    case zoal::misc::command::adc:
        stream << "\033[2K\r" << adc::read() << "\r\n";
        break;
    case zoal::misc::command::axis:
        read_axis();
        break;
    case zoal::misc::command::blink:
        scheduler.remove(toggle_led_id);
        if ((int)*argv != 0) {
            toggle_led(1);
        }
        break;
    case zoal::misc::command::help:
        stream << "\033[2K\r";
        stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
        break;
    case zoal::misc::command::i2c:
        scan_i2c();
        break;
    case zoal::misc::command::oled:
        oled_render(argv->start + 1, argv->end - 1);
        break;
    case zoal::misc::command::rgb:
        shield::rgb((int)argv[0], (int)argv[1], (int)argv[2]);
        break;
    case zoal::misc::command::rgb_off:
        shield::rgb_off();
        break;
    case zoal::misc::command::temp:
        request_temp();
        break;
    case zoal::misc::command::time:
        request_time();
        break;
    case zoal::misc::command::time_set: {
        zoal::misc::type_detector_v2 td;
        auto pos = td.detect(argv->start, argv->end, argv->end);
        if (td.type() != zoal::misc::value_type::date_time) {
            return;
        }

        auto dt = zoal::parse::type_parser<zoal::data::date_time>::parse(pos);
        clock.date_time(dt);
        clock.update(i2c_dispatcher)([](int) { terminal.sync(); });
        break;
    }
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

static void accelerometer_initialized(int) {
    stream << "\033[2K\r";
    stream << zoal::io::progmem_str(zoal_ascii_logo) << zoal::io::progmem_str(help_msg);
    terminal.sync();
}

static void display_initialized(int) {
    accelerometer.power_on(i2c_dispatcher)(accelerometer_initialized);
}

static void initialize_i2c_devices() {
    shield::ssd1306_slave_address_setup();

    oled.init(i2c_dispatcher)(display_initialized);
    i2c_dispatcher.handle_until_finished();
}

shield::joystick joystick;

static void button_handler(zoal::io::button_event e, shield::joystick_button b) {
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

    auto g = graphics::from_memory(oled.buffer.canvas);
    g->clear(0);
    oled.display(i2c_dispatcher)([](int) { terminal.sync(); });

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

        i2c_dispatcher.handle();
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
    usart::tx_handler([](uint8_t &value) { return usart_tx_type::tx_buffer.pop_front(value); });
}

ISR(TWI_vect) {
    i2c::handle_request_irq(i2c_dispatcher.request);
}

#pragma clang diagnostic pop

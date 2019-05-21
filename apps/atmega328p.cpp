#include "templates/blink.hpp"
#include "templates/compile_check.hpp"
#include "templates/ds3231.hpp"
#include "templates/ir_remove.hpp"
#include "templates/max72xx.hpp"
#include "templates/max72xx_segment.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/neo_pixel.hpp"
#include "templates/tm1637.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <avr/eeprom.h>
#include <zoal/arch/avr/atmega/i2c.hpp>
#include <zoal/arch/avr/atmega/spi.hpp>
#include <zoal/arch/avr/port.hpp>
#include <zoal/board/arduino_uno.hpp>
#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/gfx/renderer.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/io/input_stream.hpp>
#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/periph/rx_null_buffer.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/shield/uno_lcd.hpp>
#include <zoal/utils/helpers.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using adc = mcu::adc_00;
using spi = mcu::spi_00;
using i2c = mcu::i2c_00;
using irq_handler = counter::handler<mcu::frequency, 64, timer>;
using log_usart = mcu::usart_00;

using tx_buffer = log_usart::default_tx_buffer<16>;
using rx_buffer = log_usart::default_rx_buffer<16>;

using i2c_stream = zoal::periph::i2c_stream<i2c>;
using logger = zoal::utils::terminal_logger<tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = tools::delay;

using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<tools, i2c, zoal::pcb::ard_d07, zoal::pcb::ard_d08, 0x3C>;
using ssd1306 = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;
using adapter = zoal::ic::ssd1306_adapter_0<128, 64>;
using graphics = zoal::gfx::renderer<uint8_t, adapter>;

using app0 = neo_pixel<tools, zoal::pcb::ard_d13>;
using app1 = multi_function_shield<tools, zoal::pcb>;
using app2 = blink<tools, zoal::pcb::ard_d13>;
using app3 = uno_lcd_shield<tools, zoal::pcb, mcu::adc_00>;
//using app5 = max72xx<tools, mcu::mosi0, mcu::sclk0, zoal::pcb::ard_d10>;
using app6 = ir_remove<zoal::pcb::ard_d10, tools, 25>;
using app7 = tm1637<tools, zoal::pcb::ard_d10, zoal::pcb::ard_d11>;
using app8 = max72xx_segment<spi, zoal::pcb::ard_d10>;
using app9 = max72xx<tools, spi, zoal::pcb::ard_d10>;
using check = compile_check<app0, app1, app2, app3, app6, app7>;

using keypad = typename app3::shield::keypad;
using lcd = typename app3::shield::lcd;

uint8_t graphic_buffer[ssd1306::resolution_info::buffer_size];
uint8_t i2c_buffer[sizeof(i2c_stream) + 64];
auto iic_stream = i2c_stream::from_memory(i2c_buffer, sizeof(i2c_buffer));

ssd1306 display(iic_stream);

void initialize_hardware() {
    mcu::power<log_usart, timer, adc, i2c>::on();

    mcu::mux::usart<log_usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::on();
    mcu::cfg::usart<log_usart, 115200>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::on();
    mcu::cfg::i2c<i2c>::apply();

    //    mcu::cfg::spi<spi, 4>::apply();
    //    mcu::mux::spi<spi, mcu::pb_03, mcu::pb_04, mcu::pb_05, mcu::pb_02>::on();
    //    mcu::enable<spi>::on();

    mcu::cfg::adc<adc>::apply();

    mcu::enable<log_usart, timer, adc, i2c>::on();

    zoal::utils::interrupts::on();
}

using up_button = zoal::io::button<tools, zoal::pcb::ard_a01>;
using right_button = zoal::io::button<tools, zoal::pcb::ard_a02>;
using left_button = zoal::io::button<tools, zoal::pcb::ard_a03>;
using enter_button = zoal::io::button<tools, zoal::pcb::ard_a04>;
using down_button = zoal::io::button<tools, zoal::pcb::ard_a05>;

up_button up;
right_button right;
left_button left;
enter_button enter;
down_button down;

int main() {
    initialize_hardware();
    //    tools::api::merge<
    //            typename up_button::gpio_cfg
    //    >();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    logger::info() << "----- Started!! ------";

    tools::api::mode<zoal::gpio::pin_mode::input_pull_up, zoal::pcb::ard_a01, zoal::pcb::ard_a02, zoal::pcb::ard_a03, zoal::pcb::ard_a04, zoal::pcb::ard_a05>();

    //    zoal::pcb::ard_a01::mode<zoal::gpio::pin_mode::input_pull_up>();

    display.init();
    display.ensure_ready();
    memset(graphic_buffer, 0, 1024);

    auto g = graphics::from_memory(graphic_buffer);
    g->clear(0);
    g->draw_line(0, 0, 10, 10, 1);
    display.display(graphic_buffer, sizeof(graphic_buffer));

    while (true) {
        // Enter & down button uses SDA & SCL I2C pins
        // Do not read it values when I2C is working
        if (i2c::busy()) {
            continue;
        }

        up.handle([](zoal::io::button_event e) {
            if (e == zoal::io::button_event::press) {
                logger::info() << "Pressed up";
            }
        });
        left.handle([](zoal::io::button_event e) {
            if (e == zoal::io::button_event::press) {
                logger::info() << "Pressed left";
            }
        });
        right.handle([](zoal::io::button_event e) {
            if (e == zoal::io::button_event::press) {
                logger::info() << "Pressed right";
            }
        });
        down.handle([](zoal::io::button_event e) {
            if (e == zoal::io::button_event::press) {
                logger::info() << "Pressed down";
            }
        });
        enter.handle([](zoal::io::button_event e) {
            if (e == zoal::io::button_event::press) {
                logger::info() << "Pressed enter";
            }
        });
    }

    return 0;
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART_RX_vect) {
    log_usart::rx_handler_v2<rx_buffer>();
}

ISR(USART_UDRE_vect) {
    log_usart::tx_handler_v2<tx_buffer>();
}

ISR(TWI_vect) {
    i2c::handle_irq();
}

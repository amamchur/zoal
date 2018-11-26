
//#include "templates/ds3231.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <avr/eeprom.h>
#include <zoal/arch/avr/atmega/i2c.hpp>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/periph/rx_null_buffer.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

volatile uint32_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using scheduler_type = zoal::utils::function_scheduler<counter, 8, void *>;

using timer = zoal::pcb::mcu::timer_00;
using irq_handler = counter::handler<zoal::pcb::mcu::frequency, 64, timer>;
using usart = mcu::usart_01;
using usart_01_tx_buffer = zoal::periph::tx_ring_buffer<usart, 64>;
using usart_01_rx_buffer = zoal::periph::rx_null_buffer;

using adc = mcu::adc_00;
using i2c = mcu::i2c_00;
using logger = zoal::utils::plain_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, counter, logger>;
using delay = tools::delay;

using i2c_stream = zoal::periph::i2c_stream<i2c>;
using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<tools, i2c, zoal::pcb::ard_d07, zoal::pcb::ard_d08, 0x3C>;
using ssd1306 = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;

uint8_t graphic_buffer[ssd1306::resolution_info::buffer_size];
uint8_t i2c_buffer[sizeof(i2c_stream) + 64];
auto stream = i2c_stream::from_memory(i2c_buffer, sizeof(i2c_buffer));
ssd1306 ssd1306_display(stream);

template<uint8_t Width, uint8_t Height>
class ssd1306_writer {
public:
    static void ssd1306_write(void *data, int x, int y, uint8_t value) {
        if ((x < 0) || (x >= Width) || (y < 0) || (y >= Height)) {
            return;
        }

        auto buffer = reinterpret_cast<uint8_t *>(data);
        auto bit = y & 0x07;
        auto ptr = buffer + x + (y / 8) * Width;
        if (value) {
            *ptr |= 1 << bit;
        } else {
            *ptr &= ~(1 << bit);
        }
    }
};

template<uint8_t Width, uint8_t Height>
class gfx_writer_0 : public ssd1306_writer<Width, Height> {
public:
    static void write(void *data, int x, int y, uint8_t value) {
        ssd1306_writer<Width, Height>::ssd1306_write(data, x, y, value);
    }

    static void clear(void *data) {
        memset(data, 0, static_cast<size_t>(Width * Height / 8));
    }
};

template<uint8_t Width, uint8_t Height>
class gfx_writer_90 : public ssd1306_writer<Width, Height> {
public:
    static void write(void *data, int x, int y, uint8_t value) {
        ssd1306_writer<Width, Height>::ssd1306_write(data, Width - y - 1, x, value);
    }
};

template<uint8_t Width, uint8_t Height>
class gfx_writer_180 : public ssd1306_writer<Width, Height> {
public:
    static void write(void *data, int x, int y, uint8_t value) {
        ssd1306_writer<Width, Height>::ssd1306_write(data, Width - x - 1, Height - y - 1, value);
    }
};

template<uint8_t Width, uint8_t Height>
class gfx_writer_270 : public ssd1306_writer<Width, Height> {
public:
    static void write(void *data, int x, int y, uint8_t value) {
        ssd1306_writer<Width, Height>::ssd1306_write(data, y, Height - x - 1, value);
    }
};

template<class Pixel, class Writer>
class bresenham_gfx {
public:
    using self_type = bresenham_gfx<Pixel, Writer>;
    using writer = Writer;

    static self_type *from_memory(void *buffer) {
        return reinterpret_cast<self_type *>(buffer);
    }

    void clear(Pixel v) {
        writer::clear(data, 0);
    }

    void pixel(int x, int y, Pixel c) {
        writer::write(data, x, y, c);
    }

    void swap(Pixel &a, Pixel &b) {
        auto tmp = a;
        a = b;
        b = tmp;
    }

    void line(int x0, int y0, int x1, int y1, Pixel color) {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int error = (dx > dy ? dx : -dy) / 2;

        for (;;) {
            pixel(x0, y0, color);
            if (x0 == x1 && y0 == y1) {
                break;
            }

            int e = error;
            if (e > -dx) {
                error -= dy;
                x0 += sx;
            }

            if (e < dy) {
                error += dx;
                y0 += sy;
            }
        }
    }

    void circle(int x0, int y0, int radius, Pixel color) {
        int x = radius - 1;
        int y = 0;
        int dx = 1;
        int dy = 1;
        int error = dx - (radius << 1);

        while (x >= y) {
            pixel(x0 + x, y0 + y, color);
            pixel(x0 + y, y0 + x, color);
            pixel(x0 - y, y0 + x, color);
            pixel(x0 - x, y0 + y, color);
            pixel(x0 - x, y0 - y, color);
            pixel(x0 - y, y0 - x, color);
            pixel(x0 + y, y0 - x, color);
            pixel(x0 + x, y0 - y, color);

            if (error <= 0) {
                y++;
                error += dy;
                dy += 2;
            }

            if (error > 0) {
                x--;
                dx += 2;
                error += dx - (radius << 1);
            }
        }
    }

    uint8_t data[0];
};

void initialize_hardware() {
    mcu::power<usart, timer, i2c>::on();

    mcu::mux::usart<usart, zoal::pcb::ard_d00, zoal::pcb::ard_d01, mcu::pd_05>::on();
    mcu::cfg::usart<usart, 115200>::apply();

    mcu::mux::i2c<i2c, mcu::pd_01, mcu::pd_00>::on();
    mcu::cfg::i2c<i2c>::apply();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::enable<usart, timer, i2c>::on();

    zoal::utils::interrupts::on();
}

int main() {
    initialize_hardware();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    logger::info() << "ssd1306_display: " << sizeof(ssd1306_display);
    logger::info() << "i2c_buffer     : " << sizeof(i2c_buffer);
    logger::info() << "graphic_buffer : " << sizeof(graphic_buffer);

    ssd1306_display.init();

    using writer = gfx_writer_0<128, 64>;
    //    using writer_1 = gfx_writer_90<128, 64>;
    //    using writer_2 = gfx_writer_180<128, 64>;
    //    using writer_3 = gfx_writer_270<128, 64>;

    auto g = bresenham_gfx<uint8_t, writer>::from_memory(graphic_buffer);
    uint8_t radius = 1;

    while (true) {
        ssd1306_display.ensure_ready();

        memset(graphic_buffer, 0, 1024);
        g->circle(64, 32, radius, 1);
        ssd1306_display.display(graphic_buffer, sizeof(graphic_buffer));

        radius++;
        if (radius > 30) {
            radius = 1;
        }
    }
    return 0;
#pragma clang diagnostic pop
}

ISR(TIMER0_OVF_vect) {
    irq_handler::increment();
}

ISR(USART1_RX_vect) {
    usart::rx_handler<usart_01_rx_buffer>();
}

ISR(USART1_UDRE_vect) {
    usart::tx_handler<usart_01_tx_buffer>();
}

ISR(TWI_vect) {
    i2c::handle_irq();
}


//#include "templates/ds3231.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <zoal/arch/avr/atmega/i2c.hpp>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/gfx/glyph_render.hpp>
#include <zoal/gfx/renderer.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/lm75.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/io/analog_keypad.hpp>
#include <zoal/io/button.hpp>
#include <zoal/periph/rx_null_buffer.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/shields/uno_accessory_shield.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

static const unsigned char font[] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x5B, 0x4F, 0x5B, 0x3E, 0x3E, 0x6B, 0x4F, 0x6B, 0x3E, 0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 0x18, 0x3C, 0x7E, 0x3C, 0x18,
        0x1C,
        0x57, 0x7D, 0x57, 0x1C, 0x1C, 0x5E, 0x7F, 0x5E, 0x1C, 0x00, 0x18, 0x3C, 0x18, 0x00, 0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 0x00, 0x18, 0x24, 0x18, 0x00, 0xFF,
        0xE7,
        0xDB, 0xE7, 0xFF, 0x30, 0x48, 0x3A, 0x06, 0x0E, 0x26, 0x29, 0x79, 0x29, 0x26, 0x40, 0x7F, 0x05, 0x05, 0x07, 0x40, 0x7F, 0x05, 0x25, 0x3F, 0x5A, 0x3C,
        0xE7,
        0x3C, 0x5A, 0x7F, 0x3E, 0x1C, 0x1C, 0x08, 0x08, 0x1C, 0x1C, 0x3E, 0x7F, 0x14, 0x22, 0x7F, 0x22, 0x14, 0x5F, 0x5F, 0x00, 0x5F, 0x5F, 0x06, 0x09, 0x7F,
        0x01,
        0x7F, 0x00, 0x66, 0x89, 0x95, 0x6A, 0x60, 0x60, 0x60, 0x60, 0x60, 0x94, 0xA2, 0xFF, 0xA2, 0x94, 0x08, 0x04, 0x7E, 0x04, 0x08, 0x10, 0x20, 0x7E, 0x20,
        0x10,
        0x08, 0x08, 0x2A, 0x1C, 0x08, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 0x30, 0x38, 0x3E, 0x38, 0x30,
        0x06,
        0x0E, 0x3E, 0x0E, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00, 0x07, 0x00, 0x07, 0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x24,
        0x2A,
        0x7F, 0x2A, 0x12, 0x23, 0x13, 0x08, 0x64, 0x62, 0x36, 0x49, 0x56, 0x20, 0x50, 0x00, 0x08, 0x07, 0x03, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, 0x41,
        0x22,
        0x1C, 0x00, 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x80, 0x70, 0x30, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x60,
        0x60,
        0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x42, 0x7F, 0x40, 0x00, 0x72, 0x49, 0x49, 0x49, 0x46, 0x21, 0x41, 0x49, 0x4D,
        0x33,
        0x18, 0x14, 0x12, 0x7F, 0x10, 0x27, 0x45, 0x45, 0x45, 0x39, 0x3C, 0x4A, 0x49, 0x49, 0x31, 0x41, 0x21, 0x11, 0x09, 0x07, 0x36, 0x49, 0x49, 0x49, 0x36,
        0x46,
        0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x40, 0x34, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x41, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00,
        0x41,
        0x22, 0x14, 0x08, 0x02, 0x01, 0x59, 0x09, 0x06, 0x3E, 0x41, 0x5D, 0x59, 0x4E, 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x7F, 0x49, 0x49, 0x49, 0x36, 0x3E, 0x41,
        0x41,
        0x41, 0x22, 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x7F, 0x09, 0x09, 0x09, 0x01, 0x3E, 0x41, 0x41, 0x51, 0x73, 0x7F, 0x08, 0x08,
        0x08,
        0x7F, 0x00, 0x41, 0x7F, 0x41, 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, 0x7F, 0x08, 0x14, 0x22, 0x41, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x02, 0x1C, 0x02,
        0x7F,
        0x7F, 0x04, 0x08, 0x10, 0x7F, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x7F, 0x09, 0x19, 0x29, 0x46,
        0x26,
        0x49, 0x49, 0x49, 0x32, 0x03, 0x01, 0x7F, 0x01, 0x03, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x63,
        0x14,
        0x08, 0x14, 0x63, 0x03, 0x04, 0x78, 0x04, 0x03, 0x61, 0x59, 0x49, 0x4D, 0x43, 0x00, 0x7F, 0x41, 0x41, 0x41, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x41,
        0x41,
        0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x03, 0x07, 0x08, 0x00, 0x20, 0x54, 0x54, 0x78, 0x40, 0x7F, 0x28, 0x44,
        0x44,
        0x38, 0x38, 0x44, 0x44, 0x44, 0x28, 0x38, 0x44, 0x44, 0x28, 0x7F, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x08, 0x7E, 0x09, 0x02, 0x18, 0xA4, 0xA4, 0x9C,
        0x78,
        0x7F, 0x08, 0x04, 0x04, 0x78, 0x00, 0x44, 0x7D, 0x40, 0x00, 0x20, 0x40, 0x40, 0x3D, 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,
        0x7C,
        0x04, 0x78, 0x04, 0x78, 0x7C, 0x08, 0x04, 0x04, 0x78, 0x38, 0x44, 0x44, 0x44, 0x38, 0xFC, 0x18, 0x24, 0x24, 0x18, 0x18, 0x24, 0x24, 0x18, 0xFC, 0x7C,
        0x08,
        0x04, 0x04, 0x08, 0x48, 0x54, 0x54, 0x54, 0x24, 0x04, 0x04, 0x3F, 0x44, 0x24, 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x3C, 0x40,
        0x30,
        0x40, 0x3C, 0x44, 0x28, 0x10, 0x28, 0x44, 0x4C, 0x90, 0x90, 0x90, 0x7C, 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, 0x08, 0x36, 0x41, 0x00, 0x00, 0x00, 0x77,
        0x00,
        0x00, 0x00, 0x41, 0x36, 0x08, 0x00, 0x02, 0x01, 0x02, 0x04, 0x02, 0x3C, 0x26, 0x23, 0x26, 0x3C, 0x1E, 0xA1, 0xA1, 0x61, 0x12, 0x3A, 0x40, 0x40, 0x20,
        0x7A,
        0x38, 0x54, 0x54, 0x55, 0x59, 0x21, 0x55, 0x55, 0x79, 0x41, 0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
        0x21, 0x55, 0x54, 0x78, 0x40, 0x20, 0x54, 0x55, 0x79, 0x40, 0x0C, 0x1E, 0x52, 0x72, 0x12, 0x39, 0x55, 0x55, 0x55, 0x59, 0x39, 0x54, 0x54, 0x54, 0x59,
        0x39,
        0x55, 0x54, 0x54, 0x58, 0x00, 0x00, 0x45, 0x7C, 0x41, 0x00, 0x02, 0x45, 0x7D, 0x42, 0x00, 0x01, 0x45, 0x7C, 0x40, 0x7D, 0x12, 0x11, 0x12,
        0x7D, // A-umlaut
        0xF0, 0x28, 0x25, 0x28, 0xF0, 0x7C, 0x54, 0x55, 0x45, 0x00, 0x20, 0x54, 0x54, 0x7C, 0x54, 0x7C, 0x0A, 0x09, 0x7F, 0x49, 0x32, 0x49, 0x49, 0x49, 0x32,
        0x3A,
        0x44, 0x44, 0x44, 0x3A, // o-umlaut
        0x32, 0x4A, 0x48, 0x48, 0x30, 0x3A, 0x41, 0x41, 0x21, 0x7A, 0x3A, 0x42, 0x40, 0x20, 0x78, 0x00, 0x9D, 0xA0, 0xA0, 0x7D, 0x3D, 0x42, 0x42, 0x42,
        0x3D, // O-umlaut
        0x3D, 0x40, 0x40, 0x40, 0x3D, 0x3C, 0x24, 0xFF, 0x24, 0x24, 0x48, 0x7E, 0x49, 0x43, 0x66, 0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 0xFF, 0x09, 0x29, 0xF6, 0x20,
        0xC0,
        0x88, 0x7E, 0x09, 0x03, 0x20, 0x54, 0x54, 0x79, 0x41, 0x00, 0x00, 0x44, 0x7D, 0x41, 0x30, 0x48, 0x48, 0x4A, 0x32, 0x38, 0x40, 0x40, 0x22, 0x7A, 0x00,
        0x7A,
        0x0A, 0x0A, 0x72, 0x7D, 0x0D, 0x19, 0x31, 0x7D, 0x26, 0x29, 0x29, 0x2F, 0x28, 0x26, 0x29, 0x29, 0x29, 0x26, 0x30, 0x48, 0x4D, 0x40, 0x20, 0x38, 0x08,
        0x08,
        0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, 0x2F, 0x10, 0xC8, 0xAC, 0xBA, 0x2F, 0x10, 0x28, 0x34, 0xFA, 0x00, 0x00, 0x7B, 0x00, 0x00, 0x08, 0x14, 0x2A,
        0x14,
        0x22, 0x22, 0x14, 0x2A, 0x14, 0x08, 0xAA, 0x00, 0x55, 0x00, 0xAA, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x10, 0x10, 0x10, 0xFF,
        0x00,
        0x14, 0x14, 0x14, 0xFF, 0x00, 0x10, 0x10, 0xFF, 0x00, 0xFF, 0x10, 0x10, 0xF0, 0x10, 0xF0, 0x14, 0x14, 0x14, 0xFC, 0x00, 0x14, 0x14, 0xF7, 0x00, 0xFF,
        0x00,
        0x00, 0xFF, 0x00, 0xFF, 0x14, 0x14, 0xF4, 0x04, 0xFC, 0x14, 0x14, 0x17, 0x10, 0x1F, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0x1F, 0x00, 0x10,
        0x10,
        0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x10, 0x10, 0x10, 0x10, 0xF0, 0x10, 0x00, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10,
        0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0x10, 0x00, 0x00, 0x00, 0xFF, 0x14, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x1F, 0x10, 0x17, 0x00, 0x00, 0xFC,
        0x04,
        0xF4, 0x14, 0x14, 0x17, 0x10, 0x17, 0x14, 0x14, 0xF4, 0x04, 0xF4, 0x00, 0x00, 0xFF, 0x00, 0xF7, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xF7, 0x00,
        0xF7,
        0x14, 0x14, 0x14, 0x17, 0x14, 0x10, 0x10, 0x1F, 0x10, 0x1F, 0x14, 0x14, 0x14, 0xF4, 0x14, 0x10, 0x10, 0xF0, 0x10, 0xF0, 0x00, 0x00, 0x1F, 0x10, 0x1F,
        0x00,
        0x00, 0x00, 0x1F, 0x14, 0x00, 0x00, 0x00, 0xFC, 0x14, 0x00, 0x00, 0xF0, 0x10, 0xF0, 0x10, 0x10, 0xFF, 0x10, 0xFF, 0x14, 0x14, 0x14, 0xFF, 0x14, 0x10,
        0x10,
        0x10, 0x1F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
        0x00,
        0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x38, 0x44, 0x44, 0x38, 0x44, 0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
        0x7E, 0x02, 0x02, 0x06, 0x06, 0x02, 0x7E, 0x02, 0x7E, 0x02, 0x63, 0x55, 0x49, 0x41, 0x63, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x40, 0x7E, 0x20, 0x1E, 0x20,
        0x06,
        0x02, 0x7E, 0x02, 0x02, 0x99, 0xA5, 0xE7, 0xA5, 0x99, 0x1C, 0x2A, 0x49, 0x2A, 0x1C, 0x4C, 0x72, 0x01, 0x72, 0x4C, 0x30, 0x4A, 0x4D, 0x4D, 0x30, 0x30,
        0x48,
        0x78, 0x48, 0x30, 0xBC, 0x62, 0x5A, 0x46, 0x3D, 0x3E, 0x49, 0x49, 0x49, 0x00, 0x7E, 0x01, 0x01, 0x01, 0x7E, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x44, 0x44,
        0x5F,
        0x44, 0x44, 0x40, 0x51, 0x4A, 0x44, 0x40, 0x40, 0x44, 0x4A, 0x51, 0x40, 0x00, 0x00, 0xFF, 0x01, 0x03, 0xE0, 0x80, 0xFF, 0x00, 0x00, 0x08, 0x08, 0x6B,
        0x6B,
        0x08, 0x36, 0x12, 0x36, 0x24, 0x36, 0x06, 0x0F, 0x09, 0x0F, 0x06, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x30, 0x40, 0xFF, 0x01,
        0x01,
        0x00, 0x1F, 0x01, 0x01, 0x1E, 0x00, 0x19, 0x1D, 0x17, 0x12, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00};

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
using shield_type = zoal::shields::uno_accessory_shield<tools, zoal::pcb>;

uint8_t graphic_buffer[shield_type::ssd1306::resolution_info::buffer_size];
uint8_t i2c_buffer[sizeof(i2c_stream) + 64];
auto iic_stream = i2c_stream::from_memory(i2c_buffer, sizeof(i2c_buffer));

scheduler_type scheduler;
shield_type shield(iic_stream);
zoal::data::date_time current_date_time;

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

enum update_flags : uint8_t {
    screen = 0x01, rtc = 0x02, display_rtc = 0x04
};

uint8_t update_mask = screen | rtc;
char const *msg = "Test";

int x_pos = 32;
int y_pos = 32;
bool fill = true;

void render() {
    shield.display.ensure_ready();

    auto g = graphics::from_memory(graphic_buffer);
    auto f = progmem_bitmap_font<>::from_memory(font);
    zoal::gfx::glyph_render<graphics, progmem_bitmap_font<>> tl(g, f);

    g->clear(0);
    if (fill) {
        //        g->fill_rect(x_pos, y_pos, 10, 10, 1);
        g->fill_circle(x_pos, y_pos, 16, 1);
    } else {
        //        g->draw_rect(x_pos, y_pos, 10, 10, 1);
        g->draw_circle(x_pos, y_pos, 16, 1);
    }

    using reg = shield_type::ds3231::register_address;
    char text[] = "00:00:00";
    auto &rtc = shield.rtc;
    text[0] += rtc[reg::hours] >> 0x08;
    text[1] += rtc[reg::hours] & 0x0F;
    text[3] += rtc[reg::minutes] >> 0x08;
    text[4] += rtc[reg::minutes] & 0x0F;
    text[6] += rtc[reg::seconds] >> 0x08;
    text[7] += rtc[reg::seconds] & 0x0F;

    tl.position(0, 0).draw(msg, 1);
    tl.position(48, 0).draw(text, 1);

    shield.display.display(graphic_buffer, sizeof(graphic_buffer));
}

void move(int dx, int yx) {
    x_pos += dx;
    y_pos += yx;

    if (x_pos < 0) {
        x_pos = 0;
    }

    x_pos &= 0x7F;

    if (y_pos < 0) {
        y_pos = 0;
    }

    y_pos &= 0x3F;
}

void rtc_update_handler(void *) {
    update_mask |= rtc;
    scheduler.schedule(1000, rtc_update_handler);
}

void set_date_time() {
    shield.rtc.fetch(iic_stream);
    shield.rtc.wait();

    current_date_time = shield.rtc.date_time();
    current_date_time.year = 2018;
    current_date_time.month = 12;
    current_date_time.date = 5;
    current_date_time.day = 3;
    current_date_time.hours = 10;
    current_date_time.minutes = 24;
    current_date_time.seconds = 0;
    shield.rtc.date_time(current_date_time);

    shield.rtc.update(iic_stream);
    shield.rtc.wait();
}

int main() {
    initialize_hardware();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    shield.init();
    scheduler.schedule(0, rtc_update_handler);

    shield.temp_sensor.fetch(iic_stream);
    shield.temp_sensor.wait();

    logger::info() << shield.temp_sensor.temperature();

    progmem_bitmap_font<>::from_memory(font);
    while (true) {
        scheduler.handle();


        if (update_mask & screen) {
            render();
            update_mask &= ~screen;
        }

        if ((update_mask & rtc) != 0 && !i2c::busy()) {
            shield.rtc.fetch(iic_stream);
            update_mask &= ~rtc;
            update_mask |= display_rtc;
        }

        if ((update_mask & display_rtc) != 0 && shield.rtc.ready()) {
            current_date_time = shield.rtc.date_time();
            update_mask &= ~display_rtc;
            update_mask |= screen;
        }

        shield.handle_buttons([](uint8_t button, zoal::io::button_event e) {
            if (e != zoal::io::button_event::press) {
                return;
            }

            switch (button) {
            case 0:
                msg = "Up";
                move(0, -1);
                break;
            case 1:
                msg = "Left";
                move(-1, 0);
                break;
            case 2:
                msg = "Right";
                move(1, 0);
                break;
            case 3:
                msg = "Down";
                move(0, 1);
                break;
            case 4:
                fill = !fill;
                msg = "Enter";
                break;
            default:
                return;
            }

            update_mask |= screen;
        });
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

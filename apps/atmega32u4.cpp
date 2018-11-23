
//#include "templates/ds3231.hpp"
#include "templates/multi_function_shield.hpp"
#include "templates/uno_lcd_shield.hpp"

#include <avr/eeprom.h>
#include <zoal/arch/avr/atmega/i2c.hpp>
#include <zoal/board/arduino_leonardo.hpp>
#include <zoal/ic/adxl345.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/ic/max72xx.hpp>
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


#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64

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

uint8_t i2c_buffer[64];
zoal::periph::i2c_stream<i2c> stream(i2c_buffer);

zoal::ic::ds3231<> ds3231;
//zoal::ic::adxl345<i2c, 0x53, logger> adxl345;

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

uint8_t graphic_buffer[128 * 64 / 8] = {0xff};


void ssd1306_command(uint8_t c) {
    stream.write(0x3C).value(0x00).value(c);
    i2c::transmit(&stream);
    i2c::wait();
}

void display() {
    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(0);   // Column start address (0 = reset)
    ssd1306_command(SSD1306_LCDWIDTH - 1); // Column end address (127 = reset)
    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(0); // Page start address (0 = reset)
    ssd1306_command(7); // Page end address

    stream.write(0x40)
            .value(0xFF)
            .value(0xFF)
            .value(0xFF)
            .value(0xFF);
    i2c::transmit(&stream);
    i2c::wait();
}

int main() {
    initialize_hardware();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    using rst = zoal::pcb::ard_d07;
    using sa0 = zoal::pcb::ard_d08;

    rst::mode<zoal::gpio::pin_mode::output>();
    sa0::mode<zoal::gpio::pin_mode::output>();

    sa0::low(); // 0x3C

    rst::high();
    ::delay::ms(1);
    rst::low();
    ::delay::ms(10);
    rst::high();


    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
    ssd1306_command(0x80);                                  // the suggested ratio 0x80
    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
    ssd1306_command(0x3F);
    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
    ssd1306_command(0x0);                                   // no offset
    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
    ssd1306_command(0x14);
    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
    ssd1306_command(0x00);                                  // 0x0 act like ks0108
    ssd1306_command(SSD1306_SEGREMAP | 0x1);
    ssd1306_command(SSD1306_COMSCANDEC);
    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
    ssd1306_command(0x12);
    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
    ssd1306_command(0xCF);
    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
    ssd1306_command(0xF1);
    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
    ssd1306_command(0x40);
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
    ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel

    display();
    while (true) {
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

#include "../misc/max72xx.hpp"

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <zoal/mcu/attiny13a.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/utils/ms_counter.hpp>

constexpr uint8_t update_adc = 0x01u;
constexpr uint8_t update_ms = 0x02u;
constexpr uint8_t update_display = 0x04u;

using matrix_type = zoal::ic::max72xx_data<4>;

matrix_type matrix;
uint8_t message = 0;
uint8_t prev_button_value = 0;
volatile uint16_t adc_value = 0;
volatile uint8_t update_flags = update_adc | update_display;
volatile uint8_t milliseconds = 0;

using mcu = zoal::mcu::attiny13a<F_CPU>;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using adc = mcu::adc_00;
using ms_handler = zoal::utils::timer_overflow_to_tick<F_CPU, 64, 256>;
using sspi = zoal::periph::tx_software_spi<mcu::pb_03, mcu::pb_02>;
using max7219 = zoal::ic::max72xx<sspi, mcu::pb_01>;

const uint64_t messages[][4] PROGMEM = {
    {0x3f66663e66663f00, 0x0000333333336e00, 0x00003e031e301f00, 0x00003333333e301f}, // Busy
    {0x7f46161e16060f00, 0x00003b6e66060f00, 0x00001e333f031e00, 0x00001e333f031e00}, // Free
    {0x1c36636363361c00, 0x00003b66663e060f, 0x00001e333f031e00, 0x00001f3333333300}, // Open
    {0x3333331e0c0c1e00, 0x00001e333f031e00, 0x00003e031e301f00, 0x183c3c1818001800}, // Yes!
    {0x63676f7b73636300, 0x00001e3333331e00, 0x183c3c1818001800, 0x0000000000000000}, // No!
    {0x1f36666666361f00, 0x00001e3333331e00, 0x00001f3333333300, 0x00001e333f031e00}, // Done
    {0x7f46161e16060f00, 0x00001e303e336e00, 0x0c000e0c0c0c1e00, 0x0e0c0c0c0c0c1e00}, // Fail
};
constexpr auto message_count = sizeof(messages) / sizeof(messages[0]);

void fill_matrix(const void *ptr) {
    auto src = reinterpret_cast<const uint8_t *>(ptr);
    uint8_t *dest = &matrix.data[0][0];
    for (uint8_t i = 0; i < sizeof(matrix.data); i++) {
        *dest++ = pgm_read_byte(src + i);
    }
}

int main() {
    using namespace zoal::gpio;

    mcu::power<timer, adc>::on();

    mcu::cfg::adc<adc>::apply();
    mcu::mux::adc<adc, mcu::pb_04>::connect();
    mcu::irq::adc<adc>::enable();

    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64>::apply();
    mcu::irq::timer<timer>::enable_overflow_interrupt();

    mcu::enable<timer, adc>::on();

    mcu::irq::on();

//    max7219::spi::enable();
    max7219::init(matrix_type::devices);

    mcu::pb_00::mode<pin_mode::input>();
    message = eeprom_read_byte((uint8_t *)1);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        auto value = mcu::pb_00::read();
        if (value == 1 && prev_button_value != value) {
            message++;
            if (message >= message_count) {
                message = 0;
            }

            update_flags |= update_display;
            eeprom_write_byte((uint8_t *)1, message);
        }

        prev_button_value = value;

        if (update_flags & update_ms) {
            milliseconds++;
            update_flags &= ~update_ms;
        }

        if (update_flags & update_display) {
            update_flags &= ~update_display;
            fill_matrix(messages[message]);
            max7219::display(matrix);
        }

        if (update_flags & update_adc) {
            update_flags &= ~update_adc;
            max7219::send(matrix_type::devices, max7219::intensity0 | (adc_value >> 6u));
        }

        if (milliseconds == 255) {
            adc::wait();
            adc::start();
        }
    }
#pragma clang diagnostic pop

    return 0;
}

ISR(ADC_vect) {
    adc_value = adc::value();
    update_flags |= update_adc;
}

ISR(TIM0_OVF_vect) {
    update_flags |= update_ms;
}

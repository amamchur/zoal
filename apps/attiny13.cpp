#include <avr/interrupt.h>
#include <zoal/data/segment7.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/mcu/attiny13a.hpp>
#include <zoal/utils/helpers.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>

enum : uint8_t { update_adc = 0x01, update_ms = 0x02 };

using matrix_type = zoal::ic::max72xx_data<1>;

matrix_type matrix;
volatile uint16_t adc_value = 0;
volatile uint8_t update_flags = update_adc;
volatile uint8_t milliseconds = 0;

constexpr auto pwm_channel = 0;
using mcu = zoal::mcu::attiny13a<F_CPU>;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using timer = mcu::timer_00;
using adc = mcu::adc_00;
using ms_handler = counter::handler<mcu::frequency, 64, timer>;
using sspi = zoal::gpio::tx_software_spi<mcu::pb_03, mcu::pb_02>;
using max7219 = zoal::ic::max72xx<sspi, mcu::pb_01>;

int main() {
    mcu::power<timer, adc>::on();

    mcu::cfg::adc<adc>::apply();
    mcu::cfg::timer<timer, zoal::periph::timer_mode::up, 64, 1, 0xFF>::apply();
    mcu::mux::adc<adc, mcu::pb_04>::on();
    mcu::mux::timer<timer, mcu::pb_00, pwm_channel>::on();
    mcu::irq::timer<timer>::enable_overflow_interrupt();
    mcu::irq::adc<adc>::enable();
    mcu::enable<timer, adc>::on();

    zoal::utils::interrupts::on();

    max7219::spi::init();
    max7219::init(matrix_type::devices);
    max7219::send(matrix_type::devices, max7219::intensity0);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        if (update_flags & update_ms) {
            ms_handler::increment();
            update_flags &= ~update_ms;
        }

        if (update_flags & update_adc) {
            update_flags = 0;

            matrix.clear();
            auto end = zoal::utils::split_number(adc_value, &matrix.data[0][0], 10);
            if (end == &matrix.data[0][0]) {
                end++;
            }

            zoal::utils::apply(zoal::data::segment7::gfed_hex, &matrix.data[0][0], end);

            end = zoal::utils::split_number(adc_value >> 2, &matrix.data[0][4], 10);
            if (end == &matrix.data[0][4]) {
                end++;
            }

            zoal::utils::apply(zoal::data::segment7::gfed_hex, &matrix.data[0][4], end);

            max7219::display(matrix);
            timer::compare_value<pwm_channel>(adc_value >> 2);
            update_flags &= ~update_adc;
        }

        if (milliseconds > 100) {
            milliseconds = 0;
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

#include <avr/interrupt.h>

#include <zoal/board/attiny13.hpp>
#include <zoal/gpio/software_spi.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/prescalers.hpp>
#include <zoal/utils/helpers.hpp>
#include <zoal/data/segment7.hpp>
#include <zoal/ic/max72xx.hpp>

enum : uint8_t {
    update_adc = 0x01,
    update_ms = 0x02
};

using matrix_type = zoal::ic::max72xx_data<1>;

matrix_type matrix;
volatile uint16_t adc_value = 0;
volatile uint8_t update_flags = update_adc;
volatile uint8_t milliseconds = 0;

using mcu = zoal::pcb::mcu;
using counter = zoal::utils::ms_counter<decltype(milliseconds), &milliseconds>;
using ms_timer = mcu::timer0;
using ms_handler = counter::handler<mcu::frequency, 64, ms_timer>;
using sspi = zoal::gpio::tx_software_spi<mcu::pb3, mcu::pb2>;
using max7219 = zoal::ic::max72xx<sspi, mcu::pb1>;

int main() {
    asm volatile("nop"            "\n");
    asm volatile("nop"            "\n");
    asm volatile("nop"            "\n");
    mcu::api::high<mcu::pb0, mcu::pb1, mcu::pb2, mcu::pb3, mcu::pb4, mcu::pb5>::apply();
    asm volatile("nop"            "\n");
    mcu::api::low<mcu::pb0, mcu::pb1, mcu::pb2, mcu::pb3, mcu::pb4, mcu::pb5>::apply();
    asm volatile("nop"            "\n");
    mcu::api::toggle<mcu::pb0, mcu::pb1, mcu::pb2, mcu::pb3, mcu::pb4, mcu::pb5>::apply();
    asm volatile("nop"            "\n");
//
//    ms_timer::mode<zoal::periph::timer_mode::fast_pwm_8bit>();
//    ms_timer::select_clock_source<prescaler>();
//    ms_timer::enable_overflow_interrupt();
//
//    zoal::utils::interrupts::on();
//
//    mcu::adc0::setup();
//    mcu::adc0::enable();
//    mcu::adc0::enable_interrupt();
//    mcu::pb4_adc0::on();
//    mcu::pb0_timer0::on();
//
//    max7219::spi::init();
//    max7219::init(matrix_type::devices);
//    max7219::send(matrix_type::devices, max7219::intensity0);
//
//#pragma clang diagnostic push
//#pragma clang diagnostic ignored "-Wmissing-noreturn"
//    while (true) {
//        if (update_flags & update_ms) {
//            ms_handler::increment();
//            update_flags &= ~update_ms;
//        }
//
//        if (update_flags & update_adc) {
//            update_flags = 0;
//
//            matrix.clear();
//            auto end = zoal::utils::to_exponents(adc_value, &matrix.data[0][0], 10);
//            if (end == &matrix.data[0][0]) {
//                end++;
//            }
//            zoal::utils::apply(zoal::data::segment7::gfed_hex, &matrix.data[0][0], end);
//
//            end = zoal::utils::to_exponents(adc_value >> 2, &matrix.data[0][4], 10);
//            if (end == &matrix.data[0][4]) {
//                end++;
//            }
//
//            zoal::utils:: apply(zoal::data::segment7::gfed_hex, &matrix.data[0][4], end);
//
//            max7219::display(matrix);
//            mcu::pb0_timer0::value(adc_value >> 2);
//            update_flags &= ~update_adc;
//        }
//
//        if (milliseconds > 100) {
//            milliseconds = 0;
//            mcu::adc0::wait();
//            mcu::adc0::start();
//        }
//    }
#pragma clang diagnostic pop

    return 0;
}
//
//ISR(ADC_vect) {
//    adc_value = mcu::adc0::value();
//    update_flags |= update_adc;
//}
//
//ISR(TIM0_OVF_vect) {
//    update_flags |= update_ms;
//}

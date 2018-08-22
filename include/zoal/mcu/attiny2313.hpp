#ifndef ZOAL_MCU_ATTINY2313_HPP
#define ZOAL_MCU_ATTINY2313_HPP

#include <stdint.h>

#include "base_mcu.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/adc_connection.hpp"
#include "../periph/pwm_connection.hpp"
#include "../arch/avr/attiny_universe.hpp"
#include "zoal/arch/avr/adc.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class attiny2313 : public base_mcu<Frequency, 1> {
    public:
        using atu = ::zoal::arch::attiny_universe;

        template<class Controller, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Controller, Offset>;

        using port_a = atu::port_a;
        using port_b = atu::port_b;
        using port_d = atu::port_d;

        using adc0 = ::zoal::arch::avr::adc<zoal::arch::avr::mcu_type::attiny13, 0x23, 0>;

        using pecs = zoal::arch::avr::prescaler_and_ext_clock_source;
        using tifrs = zoal::arch::avr::timer_interrupt_flag_register<0x58>;
        using t0imr = zoal::arch::avr::timer_interrupt_mask_register<0x59, 1 << 1, 1 << 0, 1 << 2>;
        using t1imr = zoal::arch::avr::timer_interrupt_mask_register<0x59, 1 << 7, 1 << 6, 1 << 5>;

        static constexpr auto Timer8Type = zoal::arch::avr::mcu_type::attiny2313;
        static constexpr auto Timer16Type = zoal::arch::avr::mcu_type::attiny2313;

        using timer8_model = zoal::arch::avr::timer8_model<Timer8Type>;
        using timer16_model = zoal::arch::avr::timer16_model<Timer16Type>;

        template<uintptr_t Address, class ClkSrc, uint8_t N>
        using timer8 = typename zoal::arch::avr::timer8<timer8_model, Address, tifrs, t0imr, ClkSrc, N>;

        template<uintptr_t Address, class ClkSrc, uint8_t N>
        using timer16 = typename zoal::arch::avr::timer16<timer16_model, Address, tifrs, t1imr, ClkSrc, N>;

        using timer0 = timer8<0x50, pecs, 0>;
        using timer1 = timer16<0x44, pecs, 1>;

        template<uint8_t Channel>
        using adc_config = typename ::zoal::periph::adc_config<Channel, 0x00>;

        using pa0 = pin<port_a, 0>;
        using pa1 = pin<port_a, 1>;

        using pb0 = pin<port_b, 0>;
        using pb1 = pin<port_b, 1>;
        using pb2 = pin<port_b, 2>;
        using pb3 = pin<port_b, 3>;
        using pb4 = pin<port_b, 4>;
        using pb5 = pin<port_b, 5>;
        using pb6 = pin<port_b, 6>;
        using pb7 = pin<port_b, 7>;

        using pd0 = pin<port_d, 0>;
        using pd1 = pin<port_d, 1>;
        using pd2 = pin<port_d, 2>;
        using pd3 = pin<port_d, 3>;
        using pd4 = pin<port_d, 4>;
        using pd5 = pin<port_d, 5>;
        using pd6 = pin<port_d, 6>;

        template<class Pin, class Timer, uint8_t Channel>
        using pwm_connection = typename ::zoal::periph::pwm_connection<Pin, Timer, Channel>;

        using b2_timer0 = pwm_connection<pb2, timer0, 0>;
        using d5_timer0 = pwm_connection<pd5, timer0, 1>;
        using b3_timer1 = pwm_connection<pb3, timer1, 0>;
        using b4_timer1 = pwm_connection<pb4, timer1, 1>;

        template<class P, uint8_t Channel>
        using adc_connection = typename ::zoal::periph::adc_connection<P, adc0, Channel>;

        using port_chain = typename ::zoal::gpio::chain_builder<port_a, port_b, port_d>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
    };
}}

#endif

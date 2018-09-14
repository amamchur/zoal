#ifndef ZOAL_MCU_ATTINY13_HPP
#define ZOAL_MCU_ATTINY13_HPP

#include <stdint.h>

#include "base_mcu.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/pwm_connection.hpp"
#include "../arch/avr/attiny_universe.hpp"
#include "../arch/avr/adc.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class attiny13 : public base_mcu<Frequency, 1> {
    public:
        using atu = ::zoal::arch::attiny_universe;

        template<class Controller, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Controller, Offset>;

        using port_b = atu::port_b;
        using adc0 = ::zoal::arch::avr::adc<zoal::arch::avr::mcu_type::attiny13, 0x23, 0>;

        using timer8_model = zoal::arch::avr::timer8_mem_model<zoal::arch::avr::mcu_type::attiny13>;
        using timer0 = zoal::arch::avr::timer8<0x49, 0, false, timer8_model>;

        using pb0 = pin<port_b, 0>;
        using pb1 = pin<port_b, 1>;
        using pb2 = pin<port_b, 2>;
        using pb3 = pin<port_b, 3>;
        using pb4 = pin<port_b, 4>;
        using pb5 = pin<port_b, 5>;

        template<class Pin, uint8_t Channel>
        using pwm_connection = typename ::zoal::periph::pwm_connection<Pin, timer0, Channel>;

        using pb0_timer0 = pwm_connection<pb0, 0>;
        using pb1_timer0 = pwm_connection<pb1, 1>;

        using port_chain = typename ::zoal::gpio::chain_builder<port_b>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
    };

#if defined(F_CPU)
    using pcb = attiny13<F_CPU>;
#endif
}}

#endif

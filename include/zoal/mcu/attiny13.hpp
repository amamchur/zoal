#ifndef ZOAL_MCU_ATTINY13_HPP
#define ZOAL_MCU_ATTINY13_HPP

#include <stdint.h>

#include "base_mcu.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/adc_connection.hpp"
#include "../periph/pwm_connection.hpp"
#include "../arch/avr/attiny_universe.hpp"
#include "../arch/avr/adc_converter.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Freq>
    class attiny13 : public base_mcu<Freq, 1> {
    public:
        typedef ::zoal::arch::attiny_universe atu;

        template<class Controller, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Controller, Offset>;

        typedef atu::port_b port_b;
        typedef ::zoal::arch::avr::adc_converter<zoal::arch::avr::mcu_type::attiny13, 0x23, 0> adc0;

        typedef zoal::arch::avr::prescaler_and_ext_clock_source pecs;

        typedef zoal::arch::avr::timer_interrupt_flag_register<0x58> tifrs;
        typedef zoal::arch::avr::timer_interrupt_mask_register<0x59, 2, 4, 8> timrs;

        static constexpr auto timer8_type = zoal::arch::avr::mcu_type::attiny13;
        using timer8_model = zoal::arch::avr::timer8_model<timer8_type>;
        typedef zoal::arch::avr::timer8<timer8_model, 0x49, tifrs, timrs, pecs, 0> timer0;

        template<uint8_t Channel>
        using adc_config = typename ::zoal::periph::adc_config<Channel, 0x00>;

        typedef pin<port_b, 0> pb0;
        typedef pin<port_b, 1> pb1;
        typedef pin<port_b, 2> pb2;
        typedef pin<port_b, 3> pb3;
        typedef pin<port_b, 4> pb4;
        typedef pin<port_b, 5> pb5;

        template<class Pin, uint8_t Channel>
        using adc_connection = typename ::zoal::periph::adc_connection<Pin, adc0, Channel>;

        template<class Pin, uint8_t Channel>
        using pwm_connection = typename ::zoal::periph::pwm_connection<Pin, timer0, Channel>;

        typedef adc_connection<pb2, 0> pb5_adc0;
        typedef adc_connection<pb3, 1> pb2_adc0;
        typedef adc_connection<pb4, 2> pb4_adc0;
        typedef adc_connection<pb5, 3> pb3_adc0;

        typedef pwm_connection<pb0, 0> pb0_timer0;
        typedef pwm_connection<pb1, 1> pb1_timer0;

        template<class Controller, class Next>
        using aggregation_link = typename ::zoal::gpio::aggregation_link<Controller, Next>;

        class aggregator_chain_builder {
        protected:
            typedef aggregation_link<port_b, ::zoal::gpio::terminator> link0;
        public:
            typedef link0 type;
        };

        typedef typename aggregator_chain_builder::type gpio_chain;
    };

#if defined(F_CPU)
    typedef attiny13<F_CPU> pcb;
#endif
}}

#endif

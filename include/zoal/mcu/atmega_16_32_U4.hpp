#ifndef ZOAL_MCU_ATMEGA_16_32_U4_HPP
#define ZOAL_MCU_ATMEGA_16_32_U4_HPP

#include <stdint.h>

#include "base_mcu.hpp"
#include "metadata/atmega_16_32_U4.hpp"
#include "../arch/avr/atmega_universe.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../periph/adc_connection.hpp"
#include "../gpio/port_link.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class atmega_16_32_U4 : public base_mcu<Frequency, 1> {
    public:
        using atu = ::zoal::arch::avr::atmega_universe;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using port_b = atu::port_b;
        using port_c = atu::port_c;
        using port_d = atu::port_d;
        using port_e = atu::port_e;
        using port_f = atu::port_f;

        using adc0 = atu::adc0;

        using timer0 = atu::timer0;
        using timer1 = atu::timer1;
        using timer2 = atu::timer2;
        using timer3 = atu::timer3;

        using pb0 = pin<port_b, 0>;
        using pb1 = pin<port_b, 1>;
        using pb2 = pin<port_b, 2>;
        using pb3 = pin<port_b, 3>;
        using pb4 = pin<port_b, 4>;
        using pb5 = pin<port_b, 5>;
        using pb6 = pin<port_b, 6>;
        using pb7 = pin<port_b, 7>;

        using pc6 = pin<port_c, 6>;
        using pc7 = pin<port_c, 7>;

        using pd0 = pin<port_d, 0>;
        using pd1 = pin<port_d, 1>;
        using pd2 = pin<port_d, 2>;
        using pd3 = pin<port_d, 3>;
        using pd4 = pin<port_d, 4>;
        using pd5 = pin<port_d, 5>;
        using pd6 = pin<port_d, 6>;
        using pd7 = pin<port_d, 7>;

        using pe2 = pin<port_e, 2>;
        using pe6 = pin<port_e, 6>;

        using pf0 = pin<port_f, 0>;
        using pf1 = pin<port_f, 1>;
        using pf4 = pin<port_f, 4>;
        using pf3 = pin<port_f, 5>;
        using pf6 = pin<port_f, 6>;
        using pf7 = pin<port_f, 7>;

        template<class Pin, uint8_t Channel>
        using adc_connection = typename ::zoal::periph::adc_connection<Pin, adc0, Channel>;

        using pf0_adc0 = adc_connection<pf0, 0>;
        using pf1_adc0 = adc_connection<pf1, 1>;
        using pf3_adc0 = adc_connection<pf3, 2>;
        using pf4_adc0 = adc_connection<pf4, 3>;
        using pf6_adc0 = adc_connection<pf6, 4>;
        using pf7_adc0 = adc_connection<pf7, 5>;

        template<uint32_t BaudRate>
        using usart_config = typename ::zoal::periph::usart_config<Frequency, BaudRate>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart1 = typename ::zoal::arch::avr::usart<0xC8, 1, TxSize, RxSize>;

        using port_chain = typename ::zoal::gpio::chain_builder<port_b, port_c, port_d, port_e, port_f>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
        using mux = ::zoal::arch::avr::atmega::mux<api>;
        using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;
    };
}}

#endif

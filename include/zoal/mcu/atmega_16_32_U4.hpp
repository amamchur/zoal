#ifndef ZOAL_MCU_ATMEGA_16_32_U4_HPP
#define ZOAL_MCU_ATMEGA_16_32_U4_HPP

#include <stdint.h>

#include "base_mcu.hpp"
#include "metadata/atmega_16_32_U4.hpp"
#include "../arch/avr/atmega_universe.hpp"
#include "../arch/avr/atmega/irq.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
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

        using adc_00 = atu::adc_00;

        using timer_00 = atu::timer_00;
        using timer_01 = atu::timer_01;
        using timer_02 = atu::timer_02;
        using timer_03 = atu::timer_03;

        using pb_00 = pin<port_b, 0>;
        using pb_01 = pin<port_b, 1>;
        using pb_02 = pin<port_b, 2>;
        using pb_03 = pin<port_b, 3>;
        using pb_04 = pin<port_b, 4>;
        using pb_05 = pin<port_b, 5>;
        using pb_06 = pin<port_b, 6>;
        using pb_07 = pin<port_b, 7>;

        using pc_06 = pin<port_c, 6>;
        using pc_07 = pin<port_c, 7>;

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;
        using pd_07 = pin<port_d, 7>;

        using pe_02 = pin<port_e, 2>;
        using pe_06 = pin<port_e, 6>;

        using pf_00 = pin<port_f, 0>;
        using pf_01 = pin<port_f, 1>;
        using pf_04 = pin<port_f, 4>;
        using pf_03 = pin<port_f, 5>;
        using pf_06 = pin<port_f, 6>;
        using pf_07 = pin<port_f, 7>;

        template<class Buffer>
        using usart1 = typename ::zoal::arch::avr::usart<0xC8, 1, Buffer>;

        using port_chain = typename ::zoal::gpio::chain_builder<port_b, port_c, port_d, port_e, port_f>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
        using mux = ::zoal::arch::avr::atmega::mux<api>;
        using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;
        using irq = ::zoal::arch::avr::atmega::irq;
    };
}}

#endif

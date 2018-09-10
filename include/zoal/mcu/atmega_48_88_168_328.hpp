#ifndef ZOAL_MCU_ATMEGA_48_88_168_328_HPP
#define ZOAL_MCU_ATMEGA_48_88_168_328_HPP

#include <stdint.h> /* NOLINT */

#include "base_mcu.hpp"
#include "metadata/atmega_48_88_168_328.hpp"
#include "../arch/avr/atmega_universe.hpp"
#include "../arch/avr/atmega/irq.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/pwm_connection.hpp"
#include "../utils/tool_set.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class atmega_48_88_168_328 : public base_mcu<Frequency, 1> {
    public:
        using atu = ::zoal::arch::avr::atmega_universe;

        using port_b = atu::port_b;
        using port_c = atu::port_c;
        using port_d = atu::port_d;
        using adc_00 = atu::adc_00;
        using timer_00 = atu::timer_00;
        using timer_01 = atu::timer_01;
        using timer_02 = atu::timer_02;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using pb_00 = pin<port_b, 0>;
        using pb_01 = pin<port_b, 1>;
        using pb_02 = pin<port_b, 2>;
        using pb_03 = pin<port_b, 3>;
        using pb_04 = pin<port_b, 4>;
        using pb_05 = pin<port_b, 5>;
        using pb_06 = pin<port_b, 6>;
        using pb_07 = pin<port_b, 7>;

        using pc_00 = pin<port_c, 0>;
        using pc_01 = pin<port_c, 1>;
        using pc_02 = pin<port_c, 2>;
        using pc_03 = pin<port_c, 3>;
        using pc_04 = pin<port_c, 4>;
        using pc_05 = pin<port_c, 5>;
        using pc_06 = pin<port_c, 6>;

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;
        using pd_07 = pin<port_d, 7>;

        template<class Buffer>
        using usart_00 = typename ::zoal::arch::avr::usart<0xC0, 0, Buffer>;

        template<uintptr_t BufferSize>
        using i2c_00 = typename ::zoal::arch::avr::i2c<0xB8, BufferSize, pc_04, pc_05>;

        using mosi0 = pb_03;
        using miso0 = pb_04;
        using sclk0 = pb_05;
        using nss0 = pb_02;

        template<uint8_t Mode>
        using msbf_spi = typename ::zoal::arch::avr::msbf_spi<mosi0, miso0, sclk0, nss0, Mode>;

        template<uint8_t Mode>
        using lsbf_spi = typename ::zoal::arch::avr::lsbf_spi<mosi0, miso0, sclk0, nss0, Mode>;

        class spi0 {
        public:
            using msbf0 = msbf_spi<0>;
            using msbf1 = msbf_spi<1>;
            using msbf2 = msbf_spi<2>;
            using msbf3 = msbf_spi<3>;

            using lsbf0 = lsbf_spi<0>;
            using lsbf1 = lsbf_spi<1>;
            using lsbf2 = lsbf_spi<2>;
            using lsbf3 = lsbf_spi<3>;
        };

        using port_chain = typename ::zoal::gpio::chain_builder<port_b, port_c, port_d>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
        using mux = ::zoal::arch::avr::atmega::mux<api>;
        using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;
        using irq = ::zoal::arch::avr::atmega::irq;
    };
}}

#endif

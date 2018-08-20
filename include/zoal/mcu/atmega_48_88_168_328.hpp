#ifndef ZOAL_MCU_ATMEGA_48_88_168_328_HPP
#define ZOAL_MCU_ATMEGA_48_88_168_328_HPP

#include <stdint.h> /* NOLINT */

#include "base_mcu.hpp"
#include "../arch/avr/atmega_universe.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/adc_connection.hpp"
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
        using adc0 = atu::adc0;
        using timer0 = atu::timer0;
        using timer1 = atu::timer1;
        using timer2 = atu::timer2;

        template<class Controller, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Controller, Offset>;

        using pb0 = pin<port_b, 0>;
        using pb1 = pin<port_b, 1>;
        using pb2 = pin<port_b, 2>;
        using pb3 = pin<port_b, 3>;
        using pb4 = pin<port_b, 4>;
        using pb5 = pin<port_b, 5>;
        using pb6 = pin<port_b, 6>;
        using pb7 = pin<port_b, 7>;

        using pc0 = pin<port_c, 0>;
        using pc1 = pin<port_c, 1>;
        using pc2 = pin<port_c, 2>;
        using pc3 = pin<port_c, 3>;
        using pc4 = pin<port_c, 4>;
        using pc5 = pin<port_c, 5>;
        using pc6 = pin<port_c, 6>;

        using pd0 = pin<port_d, 0>;
        using pd1 = pin<port_d, 1>;
        using pd2 = pin<port_d, 2>;
        using pd3 = pin<port_d, 3>;
        using pd4 = pin<port_d, 4>;
        using pd5 = pin<port_d, 5>;
        using pd6 = pin<port_d, 6>;
        using pd7 = pin<port_d, 7>;

        template<class Pin, uint8_t Channel>
        using adc_connection = typename ::zoal::periph::adc_connection<Pin, adc0, Channel>;
        using pc0_adc0 = adc_connection<pc0, 0>;
        using pc1_adc0 = adc_connection<pc1, 1>;
        using pc2_adc0 = adc_connection<pc2, 2>;
        using pc3_adc0 = adc_connection<pc3, 3>;
        using pc4_adc0 = adc_connection<pc4, 4>;
        using pc5_adc0 = adc_connection<pc5, 5>;

        template<uint32_t BaudRate>
        using usart_config = typename ::zoal::periph::usart_config<Frequency, BaudRate>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart0 = typename ::zoal::arch::avr::usart<0xC0, TxSize, RxSize>;

        template<uintptr_t BufferSize>
        using i2c0 = typename ::zoal::arch::avr::i2c<0xB8, BufferSize, pc4, pc5>;

        using mosi0 = pb3;
        using miso0 = pb4;
        using sclk0 = pb5;
        using nss0 = pb2;

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
    };
}}

#endif

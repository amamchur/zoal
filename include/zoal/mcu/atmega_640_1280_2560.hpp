#ifndef ZOAL_MCU_ATMEGA_640_1280_2560_HPP
#define ZOAL_MCU_ATMEGA_640_1280_2560_HPP

#include "base_mcu.hpp"
#include "../arch/avr/atmega_universe.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"
#include "../periph/adc_connection.hpp"

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class atmega_640_1280_2560 : public base_mcu<Frequency, 1> {
    public:
        using atu = ::zoal::arch::avr::atmega_universe;

        using port_a = atu::port_a;
        using port_b = atu::port_b;
        using port_c = atu::port_c;
        using port_d = atu::port_d;
        using port_e = atu::port_e;
        using port_f = atu::port_f;
        using port_g = atu::port_g;

        using port_h = atu::port_h;
        using port_j = atu::port_j;
        using port_k = atu::port_k;
        using port_l = atu::port_l;

        using adc0 = atu::adc0;

        using timer0 = atu::timer0;
        using timer1 = atu::timer1;
        using timer2 = atu::timer2;
        using timer3 = atu::timer3;
        using timer4 = atu::timer4;
        using timer5 = atu::timer5;

        //
        // Extra data used for ADCSRB configuration
        // 0 byte - reset mask for ADCSRB
        // 1 byte - set mask for ADCSRB
        template<uint8_t Channel, uint32_t Extra>
        using adc_config = typename ::zoal::periph::adc_config<Channel, Extra>;

        static constexpr uint32_t LCG = 0x0008; // Low channels - reset MUX5 bit
        static constexpr uint32_t HCG = 0x0800; // High channels - set MUX5 bit

        template<class Controller, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Controller, Offset>;

        using pa0 = pin<port_a, 0>;
        using pa1 = pin<port_a, 1>;
        using pa2 = pin<port_a, 2>;
        using pa3 = pin<port_a, 3>;
        using pa4 = pin<port_a, 4>;
        using pa5 = pin<port_a, 5>;
        using pa6 = pin<port_a, 6>;
        using pa7 = pin<port_a, 7>;

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
        using pc7 = pin<port_c, 7>;

        using pd0 = pin<port_d, 0>;
        using pd1 = pin<port_d, 1>;
        using pd2 = pin<port_d, 2>;
        using pd3 = pin<port_d, 3>;
        using pd4 = pin<port_d, 4>;
        using pd5 = pin<port_d, 5>;
        using pd6 = pin<port_d, 6>;
        using pd7 = pin<port_d, 7>;
        using pe0 = pin<port_d, 0>;

        using pe1 = pin<port_e, 1>;
        using pe2 = pin<port_e, 2>;
        using pe3 = pin<port_e, 3>;
        using pe4 = pin<port_e, 4>;
        using pe5 = pin<port_e, 5>;
        using pe6 = pin<port_e, 6>;
        using pe7 = pin<port_e, 7>;

        using pf0 = pin<port_f, 0>;
        using pf1 = pin<port_f, 1>;
        using pf2 = pin<port_f, 2>;
        using pf3 = pin<port_f, 3>;
        using pf4 = pin<port_f, 4>;
        using pf5 = pin<port_f, 5>;
        using pf6 = pin<port_f, 6>;
        using pf7 = pin<port_f, 7>;

        using pg0 = pin<port_g, 0>;
        using pg1 = pin<port_g, 1>;
        using pg2 = pin<port_g, 2>;
        using pg3 = pin<port_g, 3>;
        using pg4 = pin<port_g, 4>;
        using pg5 = pin<port_g, 5>;
        using pg6 = pin<port_g, 6>;
        using pg7 = pin<port_g, 7>;

        using ph0 = pin<port_h, 0>;
        using ph1 = pin<port_h, 1>;
        using ph2 = pin<port_h, 2>;
        using ph3 = pin<port_h, 3>;
        using ph4 = pin<port_h, 4>;
        using ph5 = pin<port_h, 5>;
        using ph6 = pin<port_h, 6>;
        using ph7 = pin<port_h, 7>;

        using pj0 = pin<port_j, 0>;
        using pj1 = pin<port_j, 1>;
        using pj2 = pin<port_j, 2>;
        using pj3 = pin<port_j, 3>;
        using pj4 = pin<port_j, 4>;
        using pj5 = pin<port_j, 5>;
        using pj6 = pin<port_j, 6>;
        using pj7 = pin<port_j, 7>;

        using pk0 = pin<port_k, 0>;
        using pk1 = pin<port_k, 1>;
        using pk2 = pin<port_k, 2>;
        using pk3 = pin<port_k, 3>;
        using pk4 = pin<port_k, 4>;
        using pk5 = pin<port_k, 5>;
        using pk6 = pin<port_k, 6>;
        using pk7 = pin<port_k, 7>;

        using pl0 = pin<port_l, 0>;
        using pl1 = pin<port_l, 1>;
        using pl2 = pin<port_l, 2>;
        using pl3 = pin<port_l, 3>;
        using pl4 = pin<port_l, 4>;
        using pl5 = pin<port_l, 5>;
        using pl6 = pin<port_l, 6>;
        using pl7 = pin<port_l, 7>;

        using mosi0 = pb2;
        using miso0 = pb3;
        using sclk0 = pb1;
        using nss0 = pb0;

        template<class Pin, uint8_t Channel, uint32_t Extra>
        using adc_connection = typename ::zoal::periph::adc_connection<Pin, adc0, Channel, Extra>;
        
        using pf0_adc0 = adc_connection<pf0, 0x0, LCG>;
        using pf1_adc0 = adc_connection<pf1, 0x1, LCG>;
        using pf2_adc0 = adc_connection<pf2, 0x2, LCG>;
        using pf3_adc0 = adc_connection<pf3, 0x3, LCG>;
        using pf4_adc0 = adc_connection<pf4, 0x4, LCG>;
        using pf5_adc0 = adc_connection<pf5, 0x5, LCG>;
        using pf6_adc0 = adc_connection<pf6, 0x6, LCG>;
        using pf7_adc0 = adc_connection<pf7, 0x7, LCG>;

        using pk0_adc0 = adc_connection<pk0, 0x8, HCG>;
        using pk1_adc0 = adc_connection<pk1, 0x9, HCG>;
        using pk2_adc0 = adc_connection<pk2, 0xA, HCG>;
        using pk3_adc0 = adc_connection<pk3, 0xB, HCG>;
        using pk4_adc0 = adc_connection<pk4, 0xC, HCG>;
        using pk5_adc0 = adc_connection<pk5, 0xD, HCG>;
        using pk6_adc0 = adc_connection<pk6, 0xE, HCG>;
        using pk7_adc0 = adc_connection<pk7, 0xF, HCG>;

        template <uint8_t Mode>
        using msbf_spi = typename ::zoal::arch::avr::msbf_spi<mosi0, miso0, sclk0, nss0, Mode>;

        template <uint8_t Mode>
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

        template<uint32_t BaudRate>
        using usart_config = typename ::zoal::periph::usart_config<Frequency, BaudRate>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart0 = typename ::zoal::arch::avr::usart<0xC0, TxSize, RxSize>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart1 = typename ::zoal::arch::avr::usart<0xC8, TxSize, RxSize>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart2 = typename ::zoal::arch::avr::usart<0xD0, TxSize, RxSize>;

        template<uintptr_t TxSize, uintptr_t RxSize>
        using usart3 = typename ::zoal::arch::avr::usart<0x130, TxSize, RxSize>;

        template<class Controller, class Next>
        using aggregation_link = typename ::zoal::gpio::aggregation_link<Controller, Next>;

        class aggregator_chain_builder {
        protected:
            using link0 = aggregation_link<port_a, ::zoal::gpio::terminator>;
            using link1 = aggregation_link<port_b, link0>;
            using link2 = aggregation_link<port_c, link1>;
            using link3 = aggregation_link<port_d, link2>;
            using link4 = aggregation_link<port_e, link3>;
            using link5 = aggregation_link<port_f, link4>;
            using link6 = aggregation_link<port_g, link5>;
            using link7 = aggregation_link<port_h, link6>;
            using link8 = aggregation_link<port_j, link7>;
            using link9 = aggregation_link<port_k, link8>;
        public:
            using type = aggregation_link<port_l, link9>;
        };

        using gpio_chain = typename aggregator_chain_builder::type;
    };
}}

#endif

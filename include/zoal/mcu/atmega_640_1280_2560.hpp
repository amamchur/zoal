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
        typedef ::zoal::arch::avr::atmega_universe atu;

        typedef atu::port_a port_a;
        typedef atu::port_b port_b;
        typedef atu::port_c port_c;
        typedef atu::port_d port_d;
        typedef atu::port_e port_e;
        typedef atu::port_f port_f;
        typedef atu::port_g port_g;

        typedef atu::port_h port_h;
        typedef atu::port_j port_j;
        typedef atu::port_k port_k;
        typedef atu::port_l port_l;

        typedef atu::adc0 adc0;

        typedef atu::timer0 timer0;
        typedef atu::timer1 timer1;
        typedef atu::timer2 timer2;
        typedef atu::timer3 timer3;
        typedef atu::timer4 timer4;
        typedef atu::timer5 timer5;

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

        typedef pin<port_a, 0> pa0;
        typedef pin<port_a, 1> pa1;
        typedef pin<port_a, 2> pa2;
        typedef pin<port_a, 3> pa3;
        typedef pin<port_a, 4> pa4;
        typedef pin<port_a, 5> pa5;
        typedef pin<port_a, 6> pa6;
        typedef pin<port_a, 7> pa7;

        typedef pin<port_b, 0> pb0;
        typedef pin<port_b, 1> pb1;
        typedef pin<port_b, 2> pb2;

        typedef pin<port_b, 3> pb3;
        typedef pin<port_b, 4> pb4;
        typedef pin<port_b, 5> pb5;
        typedef pin<port_b, 6> pb6;
        typedef pin<port_b, 7> pb7;

        typedef pin<port_c, 0> pc0;
        typedef pin<port_c, 1> pc1;
        typedef pin<port_c, 2> pc2;
        typedef pin<port_c, 3> pc3;
        typedef pin<port_c, 4> pc4;
        typedef pin<port_c, 5> pc5;
        typedef pin<port_c, 6> pc6;
        typedef pin<port_c, 7> pc7;

        typedef pin<port_d, 0> pd0;
        typedef pin<port_d, 1> pd1;
        typedef pin<port_d, 2> pd2;
        typedef pin<port_d, 3> pd3;
        typedef pin<port_d, 4> pd4;
        typedef pin<port_d, 5> pd5;
        typedef pin<port_d, 6> pd6;
        typedef pin<port_d, 7> pd7;
        typedef pin<port_d, 0> pe0;

        typedef pin<port_e, 1> pe1;
        typedef pin<port_e, 2> pe2;
        typedef pin<port_e, 3> pe3;
        typedef pin<port_e, 4> pe4;
        typedef pin<port_e, 5> pe5;
        typedef pin<port_e, 6> pe6;
        typedef pin<port_e, 7> pe7;

        typedef pin<port_f, 0> pf0;
        typedef pin<port_f, 1> pf1;
        typedef pin<port_f, 2> pf2;
        typedef pin<port_f, 3> pf3;
        typedef pin<port_f, 4> pf4;
        typedef pin<port_f, 5> pf5;
        typedef pin<port_f, 6> pf6;
        typedef pin<port_f, 7> pf7;

        typedef pin<port_g, 0> pg0;
        typedef pin<port_g, 1> pg1;
        typedef pin<port_g, 2> pg2;
        typedef pin<port_g, 3> pg3;
        typedef pin<port_g, 4> pg4;
        typedef pin<port_g, 5> pg5;
        typedef pin<port_g, 6> pg6;
        typedef pin<port_g, 7> pg7;

        typedef pin<port_h, 0> ph0;
        typedef pin<port_h, 1> ph1;
        typedef pin<port_h, 2> ph2;
        typedef pin<port_h, 3> ph3;
        typedef pin<port_h, 4> ph4;
        typedef pin<port_h, 5> ph5;
        typedef pin<port_h, 6> ph6;
        typedef pin<port_h, 7> ph7;

        typedef pin<port_j, 0> pj0;
        typedef pin<port_j, 1> pj1;
        typedef pin<port_j, 2> pj2;
        typedef pin<port_j, 3> pj3;
        typedef pin<port_j, 4> pj4;
        typedef pin<port_j, 5> pj5;
        typedef pin<port_j, 6> pj6;
        typedef pin<port_j, 7> pj7;

        typedef pin<port_k, 0> pk0;
        typedef pin<port_k, 1> pk1;
        typedef pin<port_k, 2> pk2;
        typedef pin<port_k, 3> pk3;
        typedef pin<port_k, 4> pk4;
        typedef pin<port_k, 5> pk5;
        typedef pin<port_k, 6> pk6;
        typedef pin<port_k, 7> pk7;

        typedef pin<port_l, 0> pl0;
        typedef pin<port_l, 1> pl1;
        typedef pin<port_l, 2> pl2;
        typedef pin<port_l, 3> pl3;
        typedef pin<port_l, 4> pl4;
        typedef pin<port_l, 5> pl5;
        typedef pin<port_l, 6> pl6;
        typedef pin<port_l, 7> pl7;

        typedef pb2 mosi0;
        typedef pb3 miso0;
        typedef pb1 sclk0;
        typedef pb0 nss0;

        template<class Pin, uint8_t Channel, uint32_t Extra>
        using adc_connection = typename ::zoal::periph::adc_connection<Pin, adc0, Channel, Extra>;
        
        typedef adc_connection<pf0, 0x0, LCG> pf0_adc0;
        typedef adc_connection<pf1, 0x1, LCG> pf1_adc0;
        typedef adc_connection<pf2, 0x2, LCG> pf2_adc0;
        typedef adc_connection<pf3, 0x3, LCG> pf3_adc0;
        typedef adc_connection<pf4, 0x4, LCG> pf4_adc0;
        typedef adc_connection<pf5, 0x5, LCG> pf5_adc0;
        typedef adc_connection<pf6, 0x6, LCG> pf6_adc0;
        typedef adc_connection<pf7, 0x7, LCG> pf7_adc0;

        typedef adc_connection<pk0, 0x8, HCG> pk0_adc0;
        typedef adc_connection<pk1, 0x9, HCG> pk1_adc0;
        typedef adc_connection<pk2, 0xA, HCG> pk2_adc0;
        typedef adc_connection<pk3, 0xB, HCG> pk3_adc0;
        typedef adc_connection<pk4, 0xC, HCG> pk4_adc0;
        typedef adc_connection<pk5, 0xD, HCG> pk5_adc0;
        typedef adc_connection<pk6, 0xE, HCG> pk6_adc0;
        typedef adc_connection<pk7, 0xF, HCG> pk7_adc0;

        template <uint8_t Mode>
        using msbf_spi = typename ::zoal::arch::avr::msbf_spi<mosi0, miso0, sclk0, nss0, Mode>;

        template <uint8_t Mode>
        using lsbf_spi = typename ::zoal::arch::avr::lsbf_spi<mosi0, miso0, sclk0, nss0, Mode>;

        class spi0 {
        public:
            typedef msbf_spi<0> msbf0;
            typedef msbf_spi<1> msbf1;
            typedef msbf_spi<2> msbf2;
            typedef msbf_spi<3> msbf3;

            typedef lsbf_spi<0> lsbf0;
            typedef lsbf_spi<1> lsbf1;
            typedef lsbf_spi<2> lsbf2;
            typedef lsbf_spi<3> lsbf3;
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
            typedef aggregation_link<port_a, ::zoal::gpio::terminator> link0;
            typedef aggregation_link<port_b, link0> link1;
            typedef aggregation_link<port_c, link1> link2;
            typedef aggregation_link<port_d, link2> link3;
            typedef aggregation_link<port_e, link3> link4;
            typedef aggregation_link<port_f, link4> link5;
            typedef aggregation_link<port_g, link5> link6;
            typedef aggregation_link<port_h, link6> link7;
            typedef aggregation_link<port_j, link7> link8;
            typedef aggregation_link<port_k, link8> link9;
        public:
            typedef aggregation_link<port_l, link9> type;
        };

        typedef typename aggregator_chain_builder::type gpio_chain;
    };
}}

#endif

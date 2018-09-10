#ifndef ZOAL_MCU_ATMEGA_640_1280_2560_HPP
#define ZOAL_MCU_ATMEGA_640_1280_2560_HPP

#include "base_mcu.hpp"
#include "metadata/atmega_640_1280_2560.hpp"
#include "../arch/avr/atmega_universe.hpp"
#include "../arch/avr/atmega/irq.hpp"
#include "../gpio/pin.hpp"
#include "../gpio/base_api.hpp"
#include "../gpio/port_link.hpp"

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

        using adc_00 = atu::adc_00;

        using timer_00 = atu::timer_00;
        using timer_01 = atu::timer_01;
        using timer_02 = atu::timer_02;
        using timer_03 = atu::timer_03;
        using timer_04 = atu::timer_04;
        using timer_05 = atu::timer_05;

        static constexpr uint32_t LCG = 0x0008; // Low channels - reset MUX5 bit
        static constexpr uint32_t HCG = 0x0800; // High channels - set MUX5 bit

        template<class Controller, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Controller, Offset>;

        using pa_00 = pin<port_a, 0>;
        using pa_01 = pin<port_a, 1>;
        using pa_02 = pin<port_a, 2>;
        using pa_03 = pin<port_a, 3>;
        using pa_04 = pin<port_a, 4>;
        using pa_05 = pin<port_a, 5>;
        using pa_06 = pin<port_a, 6>;
        using pa_07 = pin<port_a, 7>;

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
        using pc_07 = pin<port_c, 7>;

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;
        using pd_07 = pin<port_d, 7>;
        using pe_00 = pin<port_d, 0>;

        using pe_01 = pin<port_e, 1>;
        using pe_02 = pin<port_e, 2>;
        using pe_03 = pin<port_e, 3>;
        using pe_04 = pin<port_e, 4>;
        using pe_05 = pin<port_e, 5>;
        using pe_06 = pin<port_e, 6>;
        using pe_07 = pin<port_e, 7>;

        using pf_00 = pin<port_f, 0>;
        using pf_01 = pin<port_f, 1>;
        using pf_02 = pin<port_f, 2>;
        using pf_03 = pin<port_f, 3>;
        using pf_04 = pin<port_f, 4>;
        using pf_05 = pin<port_f, 5>;
        using pf_06 = pin<port_f, 6>;
        using pf_07 = pin<port_f, 7>;

        using pg_00 = pin<port_g, 0>;
        using pg_01 = pin<port_g, 1>;
        using pg_02 = pin<port_g, 2>;
        using pg_03 = pin<port_g, 3>;
        using pg_04 = pin<port_g, 4>;
        using pg_05 = pin<port_g, 5>;
        using pg_06 = pin<port_g, 6>;
        using pg_07 = pin<port_g, 7>;

        using ph_00 = pin<port_h, 0>;
        using ph_01 = pin<port_h, 1>;
        using ph_02 = pin<port_h, 2>;
        using ph_03 = pin<port_h, 3>;
        using ph_04 = pin<port_h, 4>;
        using ph_05 = pin<port_h, 5>;
        using ph_06 = pin<port_h, 6>;
        using ph_07 = pin<port_h, 7>;

        using pj_00 = pin<port_j, 0>;
        using pj_01 = pin<port_j, 1>;
        using pj_02 = pin<port_j, 2>;
        using pj_03 = pin<port_j, 3>;
        using pj_04 = pin<port_j, 4>;
        using pj_05 = pin<port_j, 5>;
        using pj_06 = pin<port_j, 6>;
        using pj_07 = pin<port_j, 7>;

        using pk_00 = pin<port_k, 0>;
        using pk_01 = pin<port_k, 1>;
        using pk_02 = pin<port_k, 2>;
        using pk_03 = pin<port_k, 3>;
        using pk_04 = pin<port_k, 4>;
        using pk_05 = pin<port_k, 5>;
        using pk_06 = pin<port_k, 6>;
        using pk_07 = pin<port_k, 7>;

        using pl_00 = pin<port_l, 0>;
        using pl_01 = pin<port_l, 1>;
        using pl_02 = pin<port_l, 2>;
        using pl_03 = pin<port_l, 3>;
        using pl_04 = pin<port_l, 4>;
        using pl_05 = pin<port_l, 5>;
        using pl_06 = pin<port_l, 6>;
        using pl_07 = pin<port_l, 7>;

        using mosi0 = pb_02;
        using miso0 = pb_03;
        using sclk0 = pb_01;
        using nss0 = pb_00;

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

        template<class Buffer>
        using usart0 = typename ::zoal::arch::avr::usart<0xC0, 0, Buffer>;

        template<class Buffer>
        using usart1 = typename ::zoal::arch::avr::usart<0xC8, 1, Buffer>;

        template<class Buffer>
        using usart2 = typename ::zoal::arch::avr::usart<0xD0, 2, Buffer>;

        template<class Buffer>
        using usart3 = typename ::zoal::arch::avr::usart<0x130, 3, Buffer>;

        using port_chain = typename ::zoal::gpio::chain_builder<
                port_a,
                port_b,
                port_c,
                port_d,
                port_e,
                port_f,
                port_g,
                port_h,
                port_j,
                port_k,
                port_l>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
        using mux = ::zoal::arch::avr::atmega::mux<api>;
        using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;
        using irq = ::zoal::arch::avr::atmega::irq;
    };
}}

#endif

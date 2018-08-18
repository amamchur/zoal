#pragma once

#ifndef ZOAL_BOARD_ARDUINO_MEGA_HPP
#define ZOAL_BOARD_ARDUINO_MEGA_HPP

#include "../mcu/atmega_640_1280_2560.hpp"
#include "../arch/avr/atmega/spi.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class arduino_mega {
    public:
        using mcu = zoal::mcu::atmega_640_1280_2560<Frequency>;

        using ard_d00 = typename mcu::pe0;
        using ard_d01 = typename mcu::pe1;
        using ard_d02 = typename mcu::pe4;
        using ard_d03 = typename mcu::pe5;
        using ard_d04 = typename mcu::pg5;
        using ard_d05 = typename mcu::pe3;

        using ard_d06 = typename mcu::ph3;
        using ard_d07 = typename mcu::ph4;
        using ard_d08 = typename mcu::ph5;
        using ard_d09 = typename mcu::ph6;
        using ard_d10 = typename mcu::pb4;
        using ard_d11 = typename mcu::pb5;

        using ard_d12 = typename mcu::pb6;
        using ard_d13 = typename mcu::pb7;
        using ard_d14 = typename mcu::pj1;
        using ard_d15 = typename mcu::pj0;
        using ard_d16 = typename mcu::ph1;
        using ard_d17 = typename mcu::ph0;
        using ard_d18 = typename mcu::pd3;
        using ard_d19 = typename mcu::pd2;
        using ard_d20 = typename mcu::pd1;
        using ard_d21 = typename mcu::pd0;
        using ard_d22 = typename mcu::pa0;
        using ard_d23 = typename mcu::pa1;
        using ard_d24 = typename mcu::pa2;
        using ard_d25 = typename mcu::pa3;
        using ard_d26 = typename mcu::pa4;
        using ard_d27 = typename mcu::pa5;
        using ard_d28 = typename mcu::pa6;
        using ard_d29 = typename mcu::pa7;
        using ard_d30 = typename mcu::pc7;
        using ard_d31 = typename mcu::pc6;
        using ard_d32 = typename mcu::pc5;
        using ard_d33 = typename mcu::pc4;
        using ard_d34 = typename mcu::pc3;
        using ard_d35 = typename mcu::pc2;
        using ard_d36 = typename mcu::pc1;
        using ard_d37 = typename mcu::pc0;
        using ard_d38 = typename mcu::pd7;
        using ard_d39 = typename mcu::pg2;
        using ard_d40 = typename mcu::pg1;
        using ard_d41 = typename mcu::pg0;
        using ard_d42 = typename mcu::pl7;
        using ard_d43 = typename mcu::pl6;
        using ard_d44 = typename mcu::pl5;
        using ard_d45 = typename mcu::pl4;
        using ard_d46 = typename mcu::pl3;
        using ard_d47 = typename mcu::pl2;
        using ard_d48 = typename mcu::pl1;
        using ard_d49 = typename mcu::pl0;
        using ard_d50 = typename mcu::pb3;
        using ard_d51 = typename mcu::pb2;
        using ard_d52 = typename mcu::pb1;
        using ard_d53 = typename mcu::pb0;

        using ard_a00 = typename mcu::pf0;
        using ard_a01 = typename mcu::pf1;
        using ard_a02 = typename mcu::pf2;
        using ard_a03 = typename mcu::pf3;
        using ard_a04 = typename mcu::pf4;
        using ard_a05 = typename mcu::pf5;
        using ard_a06 = typename mcu::pf6;
        using ard_a07 = typename mcu::pf7;
        using ard_a08 = typename mcu::pk0;
        using ard_a09 = typename mcu::pk1;
        using ard_a10 = typename mcu::pk2;
        using ard_a11 = typename mcu::pk3;
        using ard_a12 = typename mcu::pk4;
        using ard_a13 = typename mcu::pk5;
        using ard_a14 = typename mcu::pk6;
        using ard_a15 = typename mcu::pk7;

        using ard_a00_adc = typename mcu::pf0_adc0;
        using ard_a01_adc = typename mcu::pf1_adc0;
        using ard_a02_adc = typename mcu::pf2_adc0;
        using ard_a03_adc = typename mcu::pf3_adc0;
        using ard_a04_adc = typename mcu::pf4_adc0;
        using ard_a05_adc = typename mcu::pf5_adc0;
        using ard_a06_adc = typename mcu::pf6_adc0;
        using ard_a07_adc = typename mcu::pf7_adc0;

        using ard_a08_adc = typename mcu::pk0_adc0;
        using ard_a09_adc = typename mcu::pk1_adc0;
        using ard_a10_adc = typename mcu::pk2_adc0;
        using ard_a11_adc = typename mcu::pk3_adc0;
        using ard_a12_adc = typename mcu::pk4_adc0;
        using ard_a13_adc = typename mcu::pk5_adc0;
        using ard_a14_adc = typename mcu::pk4_adc0;
        using ard_a15_adc = typename mcu::pk5_adc0;

        using gpio_chain = typename mcu::gpio_chain;
        using api = ::zoal::gpio::base_api<gpio_chain>;

    };

#if defined(F_CPU)
    using pcb = arduino_mega<F_CPU>;
#endif
}

#endif

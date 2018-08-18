#pragma once

#ifndef ZOAL_BOARD_ARDUINO_MEGA_HPP
#define ZOAL_BOARD_ARDUINO_MEGA_HPP

#include "../mcu/atmega_640_1280_2560.hpp"
#include "../arch/avr/atmega/spi.hpp"

namespace zoal {
    template<uint32_t Frequency>
	class arduino_mega {
	public:
		typedef zoal::mcu::atmega_640_1280_2560<Frequency> mcu;

		typedef typename mcu::pe0 ard_d00;
		typedef typename mcu::pe1 ard_d01;
		typedef typename mcu::pe4 ard_d02;
		typedef typename mcu::pe5 ard_d03;
		typedef typename mcu::pg5 ard_d04;
		typedef typename mcu::pe3 ard_d05;

		typedef typename mcu::ph3 ard_d06;
		typedef typename mcu::ph4 ard_d07;
		typedef typename mcu::ph5 ard_d08;
		typedef typename mcu::ph6 ard_d09;
		typedef typename mcu::pb4 ard_d10;
		typedef typename mcu::pb5 ard_d11;

		typedef typename mcu::pb6 ard_d12;
		typedef typename mcu::pb7 ard_d13;
		typedef typename mcu::pj1 ard_d14;
		typedef typename mcu::pj0 ard_d15;
		typedef typename mcu::ph1 ard_d16;
		typedef typename mcu::ph0 ard_d17;
		typedef typename mcu::pd3 ard_d18;
		typedef typename mcu::pd2 ard_d19;
		typedef typename mcu::pd1 ard_d20;
		typedef typename mcu::pd0 ard_d21;
		typedef typename mcu::pa0 ard_d22;
		typedef typename mcu::pa1 ard_d23;
		typedef typename mcu::pa2 ard_d24;
		typedef typename mcu::pa3 ard_d25;
		typedef typename mcu::pa4 ard_d26;
		typedef typename mcu::pa5 ard_d27;
		typedef typename mcu::pa6 ard_d28;
		typedef typename mcu::pa7 ard_d29;
		typedef typename mcu::pc7 ard_d30;
		typedef typename mcu::pc6 ard_d31;
		typedef typename mcu::pc5 ard_d32;
		typedef typename mcu::pc4 ard_d33;
		typedef typename mcu::pc3 ard_d34;
		typedef typename mcu::pc2 ard_d35;
		typedef typename mcu::pc1 ard_d36;
		typedef typename mcu::pc0 ard_d37;
		typedef typename mcu::pd7 ard_d38;
		typedef typename mcu::pg2 ard_d39;
		typedef typename mcu::pg1 ard_d40;
		typedef typename mcu::pg0 ard_d41;
		typedef typename mcu::pl7 ard_d42;
		typedef typename mcu::pl6 ard_d43;
		typedef typename mcu::pl5 ard_d44;
		typedef typename mcu::pl4 ard_d45;
		typedef typename mcu::pl3 ard_d46;
		typedef typename mcu::pl2 ard_d47;
		typedef typename mcu::pl1 ard_d48;
		typedef typename mcu::pl0 ard_d49;
		typedef typename mcu::pb3 ard_d50;
		typedef typename mcu::pb2 ard_d51;
		typedef typename mcu::pb1 ard_d52;
		typedef typename mcu::pb0 ard_d53;

		typedef typename mcu::pf0 ard_a00;
		typedef typename mcu::pf1 ard_a01;
		typedef typename mcu::pf2 ard_a02;
		typedef typename mcu::pf3 ard_a03;
		typedef typename mcu::pf4 ard_a04;
		typedef typename mcu::pf5 ard_a05;
		typedef typename mcu::pf6 ard_a06;
		typedef typename mcu::pf7 ard_a07;
		typedef typename mcu::pk0 ard_a08;
		typedef typename mcu::pk1 ard_a09;
		typedef typename mcu::pk2 ard_a10;
		typedef typename mcu::pk3 ard_a11;
		typedef typename mcu::pk4 ard_a12;
		typedef typename mcu::pk5 ard_a13;
		typedef typename mcu::pk6 ard_a14;
		typedef typename mcu::pk7 ard_a15;

		typedef typename mcu::pf0_adc0 ard_a00_adc;
		typedef typename mcu::pf1_adc0 ard_a01_adc;
		typedef typename mcu::pf2_adc0 ard_a02_adc;
		typedef typename mcu::pf3_adc0 ard_a03_adc;
		typedef typename mcu::pf4_adc0 ard_a04_adc;
		typedef typename mcu::pf5_adc0 ard_a05_adc;
		typedef typename mcu::pf6_adc0 ard_a06_adc;
		typedef typename mcu::pf7_adc0 ard_a07_adc;

		typedef typename mcu::pk0_adc0 ard_a08_adc;
		typedef typename mcu::pk1_adc0 ard_a09_adc;
		typedef typename mcu::pk2_adc0 ard_a10_adc;
		typedef typename mcu::pk3_adc0 ard_a11_adc;
		typedef typename mcu::pk4_adc0 ard_a12_adc;
		typedef typename mcu::pk5_adc0 ard_a13_adc;
		typedef typename mcu::pk4_adc0 ard_a14_adc;
		typedef typename mcu::pk5_adc0 ard_a15_adc;

		typedef typename mcu::gpio_chain gpio_chain;
		typedef ::zoal::gpio::base_api<gpio_chain> api;

	};

#if defined(F_CPU)
	typedef arduino_mega<F_CPU> pcb;
#endif
}

#endif

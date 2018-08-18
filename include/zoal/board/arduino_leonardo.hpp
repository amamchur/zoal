#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../mcu/atmega_16_32_U4.hpp"

namespace zoal {
	template<uint32_t Frequency>
	class arduino_leonardo {
	public:
		using mcu = typename mcu::atmega_16_32_U4<Frequency>;

		using ard_d00 = typename mcu::pd2;
		using ard_d01 = typename mcu::pd3;
		using ard_d02 = typename mcu::pd1;
		using ard_d03 = typename mcu::pd0;
		using ard_d04 = typename mcu::pd4;
		using ard_d05 = typename mcu::pc6;
		using ard_d06 = typename mcu::pd7;
		using ard_d07 = typename mcu::pe6;
		using ard_d08 = typename mcu::pb4;
		using ard_d09 = typename mcu::pb5;
		using ard_d10 = typename mcu::pb6;
		using ard_d11 = typename mcu::pb7;
		using ard_d12 = typename mcu::pd6;
		using ard_d13 = typename mcu::pc7;

		using ard_a00 = typename mcu::pf7;
		using ard_a01 = typename mcu::pf6;
		using ard_a02 = typename mcu::pf3;
		using ard_a03 = typename mcu::pf4;
		using ard_a04 = typename mcu::pf1;
		using ard_a05 = typename mcu::pf0;

		using ard_a00_adc = typename mcu::pf7_adc0;
        using ard_a01_adc = typename mcu::pf6_adc0;
        using ard_a02_adc = typename mcu::pf3_adc0;
        using ard_a03_adc = typename mcu::pf4_adc0;
        using ard_a04_adc = typename mcu::pf1_adc0;
        using ard_a05_adc = typename mcu::pf0_adc0;
	};

#if defined(F_CPU)
	using pcb = arduino_leonardo<F_CPU>;
#endif
}

#endif

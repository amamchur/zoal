#ifndef ZOAL_BOARD_NUCLEOF303RE_HPP
#define ZOAL_BOARD_NUCLEOF303RE_HPP

#include "../mcu/stm32f303xDxE.hpp"

namespace zoal {
	template<uint32_t Frequency>
	class nucleo_f303re {
	public:
		using mcu = typename mcu::stm32f303xDxE<Frequency>;

		using ard_d00 = typename mcu::pa03;
		using ard_d01 = typename mcu::pa02;
		using ard_d02 = typename mcu::pa10;
		using ard_d03 = typename mcu::pb03;
		using ard_d04 = typename mcu::pb05;
		using ard_d05 = typename mcu::pb04;
		using ard_d06 = typename mcu::pb10;
		using ard_d07 = typename mcu::pa08;
		using ard_d08 = typename mcu::pa09;
		using ard_d09 = typename mcu::pc07;
		using ard_d10 = typename mcu::pb06;
		using ard_d11 = typename mcu::pa07;
		using ard_d12 = typename mcu::pa06;
		using ard_d13 = typename mcu::pa05;

		using ard_a00 = typename mcu::pa00;
		using ard_a01 = typename mcu::pa01;
		using ard_a02 = typename mcu::pa04;
		using ard_a03 = typename mcu::pb00;
		using ard_a04 = typename mcu::pc01;
		using ard_a05 = typename mcu::pc00;

		using build_in_led = typename mcu::pa05;
		using build_in_button = typename mcu::pc13;
	};

	using pcb = nucleo_f303re<72000000>;
}

#endif

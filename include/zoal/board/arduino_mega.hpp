#pragma once

#ifndef ZOAL_BOARD_ARDUINO_MEGA_HPP
#define ZOAL_BOARD_ARDUINO_MEGA_HPP

#include "../arch/avr/atmega/spi.hpp"
#include "../mcu/atmega2560.hpp"
#include "../mcu/metadata/atmega_640_1280_2560.hpp"

namespace zoal {
    template<uint32_t Frequency>
    class arduino_mega {
    public:
        using mcu = zoal::mcu::atmega2560<Frequency>;

        using ard_d00 = typename mcu::pe_00;
        using ard_d01 = typename mcu::pe_01;
        using ard_d02 = typename mcu::pe_04;
        using ard_d03 = typename mcu::pe_05;
        using ard_d04 = typename mcu::pg_05;
        using ard_d05 = typename mcu::pe_03;

        using ard_d06 = typename mcu::ph_03;
        using ard_d07 = typename mcu::ph_04;
        using ard_d08 = typename mcu::ph_05;
        using ard_d09 = typename mcu::ph_06;
        using ard_d10 = typename mcu::pb_04;
        using ard_d11 = typename mcu::pb_05;

        using ard_d12 = typename mcu::pb_06;
        using ard_d13 = typename mcu::pb_07;
        using ard_d14 = typename mcu::pj_01;
        using ard_d15 = typename mcu::pj_00;
        using ard_d16 = typename mcu::ph_01;
        using ard_d17 = typename mcu::ph_00;
        using ard_d18 = typename mcu::pd_03;
        using ard_d19 = typename mcu::pd_02;
        using ard_d20 = typename mcu::pd_01;
        using ard_d21 = typename mcu::pd_00;
        using ard_d22 = typename mcu::pa_00;
        using ard_d23 = typename mcu::pa_01;
        using ard_d24 = typename mcu::pa_02;
        using ard_d25 = typename mcu::pa_03;
        using ard_d26 = typename mcu::pa_04;
        using ard_d27 = typename mcu::pa_05;
        using ard_d28 = typename mcu::pa_06;
        using ard_d29 = typename mcu::pa_07;
        using ard_d30 = typename mcu::pc_07;
        using ard_d31 = typename mcu::pc_06;
        using ard_d32 = typename mcu::pc_05;
        using ard_d33 = typename mcu::pc_04;
        using ard_d34 = typename mcu::pc_03;
        using ard_d35 = typename mcu::pc_02;
        using ard_d36 = typename mcu::pc_01;
        using ard_d37 = typename mcu::pc_00;
        using ard_d38 = typename mcu::pd_07;
        using ard_d39 = typename mcu::pg_02;
        using ard_d40 = typename mcu::pg_01;
        using ard_d41 = typename mcu::pg_00;
        using ard_d42 = typename mcu::pl_07;
        using ard_d43 = typename mcu::pl_06;
        using ard_d44 = typename mcu::pl_05;
        using ard_d45 = typename mcu::pl_04;
        using ard_d46 = typename mcu::pl_03;
        using ard_d47 = typename mcu::pl_02;
        using ard_d48 = typename mcu::pl_01;
        using ard_d49 = typename mcu::pl_00;
        using ard_d50 = typename mcu::pb_03;
        using ard_d51 = typename mcu::pb_02;
        using ard_d52 = typename mcu::pb_01;
        using ard_d53 = typename mcu::pb_00;

        using ard_a00 = typename mcu::pf_00;
        using ard_a01 = typename mcu::pf_01;
        using ard_a02 = typename mcu::pf_02;
        using ard_a03 = typename mcu::pf_03;
        using ard_a04 = typename mcu::pf_04;
        using ard_a05 = typename mcu::pf_05;
        using ard_a06 = typename mcu::pf_06;
        using ard_a07 = typename mcu::pf_07;
        using ard_a08 = typename mcu::pk_00;
        using ard_a09 = typename mcu::pk_01;
        using ard_a10 = typename mcu::pk_02;
        using ard_a11 = typename mcu::pk_03;
        using ard_a12 = typename mcu::pk_04;
        using ard_a13 = typename mcu::pk_05;
        using ard_a14 = typename mcu::pk_06;
        using ard_a15 = typename mcu::pk_07;

        using port_chain = typename mcu::port_chain;
        using api = ::zoal::gpio::base_api<port_chain>;
    };

#if defined(F_CPU)
    using pcb = arduino_mega<F_CPU>;
#endif
}

#endif

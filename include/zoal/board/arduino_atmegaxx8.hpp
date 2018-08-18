#ifndef ZOAL_BOARD_ARDUINO_ATMAGEXX8_HPP
#define ZOAL_BOARD_ARDUINO_ATMAGEXX8_HPP

#include "../periph/adc_connection.hpp"
#include "../periph/pwm_connection.hpp"
#include "../mcu/atmega_48_88_168_328.hpp"

namespace zoal { namespace board {
    template<uint32_t Frequency>
    class arduino_atmegaxx8 {
    public:
        using mcu = typename mcu::atmega_48_88_168_328<Frequency>;

        using ard_d00 = typename mcu::pd0;
        using ard_d01 = typename mcu::pd1;
        using ard_d02 = typename mcu::pd2;
        using ard_d03 = typename mcu::pd3;
        using ard_d04 = typename mcu::pd4;
        using ard_d05 = typename mcu::pd5;
        using ard_d06 = typename mcu::pd6;
        using ard_d07 = typename mcu::pd7;
        using ard_d08 = typename mcu::pb0;
        using ard_d09 = typename mcu::pb1;
        using ard_d10 = typename mcu::pb2;
        using ard_d11 = typename mcu::pb3;
        using ard_d12 = typename mcu::pb4;
        using ard_d13 = typename mcu::pb5;

        using ard_a00 = typename mcu::pc0;
        using ard_a01 = typename mcu::pc1;
        using ard_a02 = typename mcu::pc2;
        using ard_a03 = typename mcu::pc3;
        using ard_a04 = typename mcu::pc4;
        using ard_a05 = typename mcu::pc5;

        using ard_a00_adc = typename mcu::pc0_adc0;
        using ard_a01_adc = typename mcu::pc1_adc0;
        using ard_a02_adc = typename mcu::pc2_adc0;
        using ard_a03_adc = typename mcu::pc3_adc0;
        using ard_a04_adc = typename mcu::pc4_adc0;
        using ard_a05_adc = typename mcu::pc5_adc0;
    };
}}

#endif

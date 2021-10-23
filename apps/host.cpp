#include "../misc/cmd_argument.hpp"
#include "../misc/cmd_line_parser.hpp"
#include "../misc/timer_freq_calculator.hpp"
#include "../misc/type_detector.hpp"
#include "./atmega32u4/command_machine.hpp"

#include <cstdint>
#include <iostream>
#include <zoal/ct/type_list.hpp>
#include <zoal/mcu/stm32f401ccux.hpp>

using mcu = zoal::mcu::stm32f401ccux;

using timer = mcu::timer_02;
using pin = mcu::pa_01;
using ch = mcu::mux::pwm_channel<timer, pin>;
using builder = zoal::arch::stm32f4::pwm_channel_builder<mcu::signature, timer, pin>;
using ch_types = builder::ch_maps;

class M {
public:
    template<class T>
    class match {
    public:
        static constexpr bool value = T::value >= 0;
    };
};

using iot = zoal::ct::index_of_type<M, ch_types>;
int v = iot::result;

int main() {
    std::cout << "ch1_map: " << builder::ch1_map::value << std::endl;
    std::cout << "ch2_map: " << builder::ch2_map::value << std::endl;
    std::cout << "ch3_map: " << builder::ch3_map::value << std::endl;
    std::cout << "ch4_map: " << builder::ch4_map::value << std::endl;

    std::cout << "m ch1_map: " << M::match<builder::ch1_map>::value << std::endl;
    std::cout << "m ch2_map: " << M::match<builder::ch2_map>::value << std::endl;
    std::cout << "m ch3_map: " << M::match<builder::ch3_map>::value << std::endl;
    std::cout << "m ch4_map: " << M::match<builder::ch4_map>::value << std::endl;


    std::cout << "Channel: " << iot::result << std::endl;
    return 0;
}

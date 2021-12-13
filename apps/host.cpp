#include "../misc/cmd_argument.hpp"
#include "../misc/cmd_line_parser.hpp"
#include "../misc/timer_freq_calculator.hpp"
#include "../misc/type_detector.hpp"
#include "./atmega32u4/command_machine.hpp"

#include <cstdint>
#include <iostream>
#include <zoal/ct/type_list.hpp>
#include <zoal/mcu/stm32f401ccux.hpp>
#include <zoal/periph/i2c.hpp>

using mcu = zoal::mcu::stm32f401ccux;
constexpr uint32_t apb1_clock_freq = 42000000;
using i2c = mcu::i2c_01;
using i2c_clk = mcu::pb_08;
using i2c_sda = mcu::pb_09;
using api = mcu::api;
using i2c_params = zoal::periph::i2c_fast_mode<apb1_clock_freq>;
using i2c_mux = mcu::mux::i2c<i2c, i2c_sda, i2c_clk>;
using i2c_cfg = mcu::cfg::i2c<i2c, i2c_params>;

using motor_dir = mcu::pb_02;

using case0 = api::optimize<i2c_mux::connect>;
using case1 = api::optimize<i2c_mux::connect, api::mode<zoal::gpio::pin_mode::output, motor_dir>>;
using case2 = api::optimize<api::mode<zoal::gpio::pin_mode::output, motor_dir>, i2c_mux::connect>;

using a = api::perform_cas_optimization<zoal::mem::null_cas_list, i2c_mux::connect, api::mode<zoal::gpio::pin_mode::output, motor_dir>>;

class funct {
public:
    static void print_cas(uintptr_t addr, uintptr_t c, uintptr_t s) {
        std::cout << "A: " << (void *)addr << " C: " << (void *)c << " S: " << (void *)s << "\r\n";
    }

    template<class T>
    void operator()() {
        using cas = typename T::type;
        print_cas(cas::address, cas::clear, cas::set);
    }
};

int main() {
    funct f;

    std::cout << std::endl;
    zoal::ct::type_chain_iterator<case0>::for_each(f);
    std::cout << std::endl;
    zoal::ct::type_chain_iterator<case1>::for_each(f);
    std::cout << std::endl;
    zoal::ct::type_chain_iterator<case2>::for_each(f);

    return 0;
}

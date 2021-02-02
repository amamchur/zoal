#include <iostream>
#include <zoal/func/function.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/periph/i2c.hpp>

using mcu = zoal::mcu::stm32f103c8tx;
using i2c = mcu::i2c_02;
using i2c_02_cfg = zoal::periph::i2c_config<36000000, 400000>;
using i2c_cfg = mcu::cfg::i2c<i2c, i2c_02_cfg>;

int main() {
    std::cout << "max_rise_time:\t\t\t" << i2c_cfg::max_rise_time << std::endl;
    std::cout << "t_i2c_ns:\t\t\t\t" << i2c_cfg::t_i2c_ns << std::endl;
    std::cout << "t_pclk_ns:\t\t\t\t" << i2c_cfg::t_pclk_ns << std::endl;
    std::cout << "standard_mode_ccr:\t\t" << i2c_cfg::standard_mode_ccr << std::endl;
    std::cout << "fast_mode_duty_0_ccr:\t" << i2c_cfg::fast_mode_duty_0_ccr << std::endl;
    std::cout << "fast_mode_duty_1_ccr:\t" << i2c_cfg::fast_mode_duty_1_ccr << std::endl;
    std::cout << "duty_0_accuracy:\t\t" << i2c_cfg::duty_0_accuracy << std::endl;
    std::cout << "duty_1_accuracy:\t\t" << i2c_cfg::duty_1_accuracy << std::endl;
    std::cout << "duty:\t\t\t\t\t" << i2c_cfg::duty << std::endl;
    std::cout << "ccr:\t\t\t\t\t" << i2c_cfg::ccr << std::endl;

    return 0;
}

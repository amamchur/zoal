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
#include <memory>
#include <utility>
#include <fstream>

#include <zoal/fs/fat32.hpp>

int main() {
    char test[] = {'A', 'B', 0, 0, 0, 0};
    std::string str(test, test + sizeof(test));
    std::cout << str << std::endl;

    std::fstream s{"D:/fat32.img.001", std::fstream::binary | std::fstream::in};
    zoal::fs::volume_boot_sector bs;
    s.seekg(0);
    s.read(reinterpret_cast<char*>(&bs), sizeof(zoal::fs::volume_boot_sector));


    std::string oem_name(bs.oem_name, bs.oem_name + sizeof(bs.oem_name));
    std::cout << "oem_name:\t" << oem_name << std::endl;

    return 0;
}

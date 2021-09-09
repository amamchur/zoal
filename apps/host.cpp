#include "../misc/cmd_argument.hpp"
#include "../misc/cmd_line_parser.hpp"
#include "../misc/timer_freq_calculator.hpp"
#include "../misc/type_detector.hpp"
#include "./atmega32u4/command_machine.hpp"

#include <cstdint>
#include <iostream>
#include <zoal/ct/type_list.hpp>
#include <zoal/data/vector.hpp>
#include <zoal/func/function.hpp>
#include <zoal/parse/type_parser.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/scheduler.hpp>

char test[] = "oled \"Hello\"";

void cb(zoal::misc::command_machine *m, zoal::misc::command c, int argc, zoal::misc::cmd_arg *argv) {
    std::cout << "cmd: " << (int)c << std::endl;
    std::cout << "args: " << argc << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << "s: " << argv[i].start << std::endl;

        std::string str(argv[i].start, argv[i].end);
        std::cout << "a: " <<  str << std::endl;
    }
}

int main() {
    std::cout << (void *)"test" << std::endl;
    std::cout << (void *)"test" << std::endl;

    auto s = test;
    auto e = s + strlen(test);
    zoal::misc::command_machine tp;
    tp.callback(cb);
    tp.run_machine(s, e, e);
    return 0;
}

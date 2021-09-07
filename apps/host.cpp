#include "../misc/cmd_argument.hpp"
#include "../misc/cmd_line_parser.hpp"
#include "../misc/timer_freq_calculator.hpp"
#include "../misc/type_detector.hpp"
#include "../misc/test_machine.hpp"

#include <cstdint>
#include <iostream>
#include <zoal/ct/type_list.hpp>
#include <zoal/data/vector.hpp>
#include <zoal/func/function.hpp>
#include <zoal/parse/type_parser.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/scheduler.hpp>

zoal::misc::test_parser tp(nullptr, 0);

char test[] = "step -115";

void cb(zoal::misc::test_machine *m, int c, int argc, zoal::misc::test_argument *argv) {
    std::cout << "cmd: " << c << std::endl;
    std::cout << "args: " << argc << std::endl;

    for (int i = 0; i < argc; i++) {
        std::string str(argv[i].start, argv[i].end);
        std::cout << "a: " <<  str << std::endl;
    }
}

int main() {
    tp.callback(cb);
    tp.run_machine(test, test + sizeof(test) - 1);
    return 0;
}

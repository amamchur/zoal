#include "../misc/terminal_input.hpp"

#include <iostream>
#include <zoal/utils/scheduler.hpp>
#include <zoal/algorithm/kmp.hpp>

using namespace std;

intptr_t prefix_buffer[128];

const char pattern[] = "abc";
const char test[] = "test\n abc test";
uintptr_t buffer[128];

int main() {
    using namespace std;

//    zoal::algorithm::kmp_pattern<const char *> p(pattern, buffer, sizeof(pattern) - 1);

    auto p = zoal::algorithm::make_kmp_pattern(pattern, buffer, sizeof(pattern) - 1);

    zoal::algorithm::kmp k;
    auto result = k.search(p, test, test + sizeof(test) - 1);
    std::cout << "result: " << result << std::endl;

    auto r = zoal::algorithm::kmp("test\n abc test", "abc", prefix_buffer);
    std::cout << "result: " << r << std::endl;

    for (int i = 0; i < 5; ++i) {
        std::cout << "pf 1: " << buffer[i] << ", pf 2: " << prefix_buffer[i] << std::endl;
    }

    return 0;
}

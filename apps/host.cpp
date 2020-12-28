#include "terminal_input.hpp"

#include <iostream>
#include <unistd.h>

void vt100_callback(terminal_input *t, const char *s) {
    for (auto ch = s; *ch; ch++) {
        std::cout << *ch;
    }
    std::cout.flush();
    sleep(1);
}

char buffer[128];

int main() {
    using namespace std;

    char str[] = {126};
    char size = sizeof(str);
    terminal_input tm(buffer, sizeof(buffer));
    tm.vt100_callback(&vt100_callback);

    for (int i = 0; i < size; i++) {
        tm.push(str + i, 1);
    }
    return 0;
}

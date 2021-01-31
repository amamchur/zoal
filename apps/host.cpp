#include <iostream>
#include <zoal/func/function.hpp>

template<class T>
void print_hex(T value) {
    constexpr uint8_t nibbles = sizeof(value) << 1;

    std::cout << "0x";
    for (int i = nibbles - 1; i >= 0; i--) {
        auto h = (value >> (i << 2)) & 0xF;
        char ch = h < 10 ? ('0' + h) : ('A' + h - 10);
        std::cout << ch;
    }
}

int main() {
    print_hex(-2);
    std::cout << std::endl;
    return 0;
}

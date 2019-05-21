#include <iostream>
#include <sstream>
#include <zoal/ic/max72xx.hpp>

const uint64_t messages[][4] = {
    {0x3f66663e66663f00, 0x0000333333336e00, 0x00003e031e301f00, 0x00003333333e301f}, // Busy
    {0x7f46161e16060f00, 0x00003b6e66060f00, 0x00001e333f031e00, 0x00001e333f031e00}, // Free
    {0x1c36636363361c00, 0x00003b66663e060f, 0x00001e333f031e00, 0x00001f3333333300}, // Open
    {0x3333331e0c0c1e00, 0x00001e333f031e00, 0x00003e031e301f00, 0x183c3c1818001800}, // Yes!
    {0x63676f7b73636300, 0x00001e3333331e00, 0x183c3c1818001800, 0x0000000000000000}, // No!
    {0x1f36666666361f00, 0x00001e3333331e00, 0x00001f3333333300, 0x00001e333f031e00}, // Done
    {0x7f46161e16060f00, 0x00001e303e336e00, 0x0c000e0c0c0c1e00, 0x0e0c0c0c0c0c1e00}, // Fail
};
//constexpr auto message_count = sizeof(messages) / sizeof(messages[0]);

using matrix_type = zoal::ic::max72xx_data<4>;
matrix_type matrix;

//const int has_nukes        = 0b00000000000000000000000000000001;

void fill_matrix(const void *ptr) {
    auto src = reinterpret_cast<const uint8_t *>(ptr);
    uint8_t *dest = &matrix.data[0][0];
    for (uint8_t i = 0; i < sizeof(matrix.data); i++) {
        *dest++ = *(src + i);
    }
}

void print_matrix() {
    for (int j = 7; j >= 0; j--) {
        std::stringstream ss;

        for (auto & i : matrix.data) {
            uint8_t dest = i[j];
            for (int k = 0; k < 8; k++) {
                int v = (dest >> k) & 1;
                ss << (v == 0 ? ' ' : '#');
            }
        }

        std::cout << ss.str() << std::endl;
    }
}

template<class T>
T shiftRight(T value, uint8_t, uint8_t, matrix_type &) {
    return static_cast<T>(value >> 3u);
}

void qqq(uint64_t value) {
    for (int i = sizeof(value) - 1; i >= 0; i--) {
        auto ww = value >> (i * 8);
        auto bt = static_cast<uint8_t>(ww & 0xFF);
        for (int j = 0; j < 8; j++) {
            std::cout << (((bt >> j) & 1) == 0 ? '.' : '#');
        }

        std::cout << std::endl;
    }
}

int main() {
    qqq(0x00AA00FF);

//    for (const auto &message : messages) {
//        fill_matrix(message);
//        print_matrix();
//    }
    return 0;
}

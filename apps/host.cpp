#include "data/fonts.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <zoal/ic/max72xx.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/board/arduino_uno.hpp>

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

using matrix_type = zoal::ic::max72xx_data<8>;
matrix_type matrix;

//const int has_nukes        = 0b00000000000000000000000000000001;

void fill_matrix(const void *ptr) {
    auto src = reinterpret_cast<const uint8_t *>(ptr);
    uint8_t *dest = &matrix.data[0][0];
    for (uint8_t i = 0; i < sizeof(matrix.data); i++) {
        *dest++ = *(src + i);
    }
}

void print_matrix(int from, int to) {
    for (int j = 7; j >= 0; j--) {
        std::stringstream ss;

        for (int i = from; i < to; i++) {
            uint8_t dest = matrix.data[i][j];
            for (int k = 0; k < 8; k++) {
                int v = (dest >> k) & 1;
                ss << (v == 0 ? '.' : '#');
            }
        }

        std::cout << ss.str() << std::endl;
    }
}

void fill_text(const char *ch) {
    matrix.clear();

    for (int i = 0; i < (int)matrix_type::devices && *ch; i++, ch++) {
        auto src = reinterpret_cast<const uint8_t *>(font_glyphs + (*ch - start_glyph_code));
        auto *dest = matrix.data[i];
        for (uint8_t j = 0; j < 8; j++) {
            *dest++ = *(src + j);
        }
    }
}

void display_glyphs() {
    char msg[] = "A";
    msg[0] = start_glyph_code;
    for (int i = 0; i < (int)font_glyph_count; i++) {
        msg[0] = start_glyph_code + i;
        fill_text(msg);
        std::cout << "========== Gryph: \"" << msg << "\" ==========" << std::endl;
        print_matrix(0, 1);
    }
}

const uint64_t IMAGES[] = {
        0x00380c0c070c0c38,
        0x0018181818181818,
        0x00070c0c380c0c07,
        0x0000000000003b6e
};
const int IMAGES_LEN = sizeof(IMAGES)/8;



uint64_t transform_glyph(uint64_t g) {
    uint64_t r = 0;
    for (int i = 0; i < 8; i++) {
        r <<= 8;
        r |= (g & 0xff);
        g >>= 8;
    }
    return r;
}

void transform() {
    std::cout << "Count: " << IMAGES_LEN << std::endl;
    for (int i = 0; i < IMAGES_LEN; i++) {
        uint64_t g = transform_glyph(IMAGES[i]);
        std::cout << "0x0" << std::hex << std::setfill('0') << std::setw(16) << g << "," << std::endl;
    }
}

volatile uint32_t timer0_millis = 0;
using ms_counter = zoal::utils::ms_counter<decltype(timer0_millis), &timer0_millis>;
using tools = zoal::utils::tool_set<zoal::pcb::mcu, ms_counter, void>;
tools::function_scheduler<4> timeout;

void test_timeout() {
    timeout.clear();
    timeout.schedule(0, test_timeout);
}

int main() {
//    transform();
//    fill_text("Hello!!!");
//    print_matrix(0, 2);
//    display_glyphs();
    timer0_millis = 10;
    timeout.schedule(5, test_timeout);
    timeout.schedule(20, test_timeout);
    timeout.handle();

    timer0_millis = 100;
    timeout.handle();
    return 0;
}

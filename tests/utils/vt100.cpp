#include "gtest/gtest.h"
#include <zoal/io/output_stream.hpp>
#include <zoal/utils/vt100.hpp>

#include "../test_utils/mem_transport.hpp"

using namespace zoal::tests;
using namespace zoal::utils::vt100;

TEST(vt100, color) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << color(3, 9) << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[49;33mText\x1B[m\r\n");
}


TEST(vt100, color_defsult_ctor) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << color() << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[0;0mText\x1B[m\r\n");
}

TEST(vt100, color_copy_ctor) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    color c(-1, 9);
    stream << color(c) << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[49;0mText\x1B[m\r\n");
}

TEST(vt100, cup) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << cursor_position(0, 0) << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[0;0HText\x1B[m\r\n");
}

TEST(vt100, el2) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << el2() << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[2kText\x1B[m\r\n");
}

TEST(vt100, ed2) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << ed2() << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B[2jText\x1B[m\r\n");
}

TEST(vt100, ris) {
    char buffer[256] = {0};
    mem_transport mt(buffer, sizeof(buffer));
    zoal::io::output_stream<mem_transport> stream(mt);
    stream << ris() << "Text" << sgr0() << cr_lf();
    EXPECT_STREQ(buffer, "\x1B" "cText\x1B[m\r\n");
}

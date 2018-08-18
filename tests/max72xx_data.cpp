#include "gtest/gtest.h"
#include <zoal/ic/max72xx.hpp>
#include <zoal/data/segment7.hpp>

TEST(max72xx_data, GPIO_xxx) {
    zoal::ic::max72xx_data<1> matrix;
    long prev = 1200;
    matrix.print(zoal::data::segment7::gfed_ascii, prev);

    EXPECT_EQ(matrix.data[0][0], 126);
    EXPECT_EQ(matrix.data[0][1], 126);
    EXPECT_EQ(matrix.data[0][2], 109);
    EXPECT_EQ(matrix.data[0][3], 48);
    EXPECT_EQ(matrix.data[0][4], 0);
    EXPECT_EQ(matrix.data[0][5], 0);
    EXPECT_EQ(matrix.data[0][6], 0);
    EXPECT_EQ(matrix.data[0][7], 0);
}
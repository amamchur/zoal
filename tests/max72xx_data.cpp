#include "gtest/gtest.h"
#include <zoal/ic/max72xx.hpp>
#include <zoal/data/segment7.hpp>

TEST(max72xx_data, GPIO_xxx) {
    zoal::ic::max72xx_data<1> matrix;
    uint32_t prev = 1200;
    matrix.print(zoal::data::segment7::gfed_ascii, prev);
}
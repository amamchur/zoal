#include "utils/address_cast.hpp"

#include "gtest/gtest.h"
#include <zoal/mem/cas.hpp>

TEST(ClearAndSet, static_type_checking) {
    using namespace zoal::mem;
    zoal::test::mem<void>::clear_mem();

    uint8_t *m = zoal::test::mem<void>::avr();
    cas<0, reg_io::read_write, uint8_t, 0xFF, 0, 0>();
    EXPECT_EQ(m[0], 0x00);

    cas<0, reg_io::read_write, uint8_t, 0xFF, 0, 1>();
    EXPECT_EQ(m[0], 0x01);

    cas<0, reg_io::read_write, uint8_t, 0xFF, 1, 0>();
    EXPECT_EQ(m[0], 0x00);

    cas<0, reg_io::read_write, uint8_t, 0xFF, 1, 1>();
    EXPECT_EQ(m[0], 0x01);

    cas<0, reg_io::read_write, uint8_t, 0xFF, 1, 3>();
    EXPECT_EQ(m[0], 0x03);

    cas<0, reg_io::write, uint8_t, 0xFF, 0, 0>();
    EXPECT_EQ(m[0], 0x03);

    cas<0, reg_io::write, uint8_t, 0xFF, 0, 4>();
    EXPECT_EQ(m[0], 0x04);

    cas<0, reg_io::read, uint8_t, 0xFF, 0, 0>();
    EXPECT_EQ(m[0], 0x04);
}

#include "utils/address_cast.hpp"

#include "gtest/gtest.h"
#include <zoal/mem/cas.hpp>

using namespace zoal::mem;

TEST(clear_and_set, strategy_ignore) {
    using st = cas_strategy<reg_io::read_write, 0xFF, 0, 0>;
    auto type = st::strategy;
    EXPECT_EQ(type, cas_strategy_type::ignore);
}

TEST(clear_and_set, strategy_ignore_imp) {
    using imp = cas_strategy_implementation<cas_strategy_type::ignore>;
    uintptr_t reg = 0xDEADBEEF;
    imp::apply(reg, static_cast<uintptr_t>(0xF0), static_cast<uintptr_t>(0x0F));
    EXPECT_EQ(reg, 0xDEADBEEF);
}

TEST(clear_and_set, strategy_set) {
    using st = cas_strategy<reg_io::read_write, 0xFF, 0, 1>;
    auto type = st::strategy;
    EXPECT_EQ(type, cas_strategy_type::set);

    using st2 = cas_strategy<reg_io::read_write, 0xFF, 5, 5>;
    type = st2::strategy;
    EXPECT_EQ(type, cas_strategy_type::set);
}

TEST(clear_and_set, strategy_set_imp) {
    using imp = cas_strategy_implementation<cas_strategy_type::set>;
    uintptr_t reg = 0xCAFED00D;
    imp::apply(reg, static_cast<uintptr_t>(0xD00D), static_cast<uintptr_t>(0x0F));
    EXPECT_EQ(reg, 0xCAFED00F);
}

TEST(clear_and_set, strategy_clear) {
    using st = cas_strategy<reg_io::read_write, 0xFF, 1, 0>;
    auto type = st::strategy;
    EXPECT_EQ(type, cas_strategy_type::clear);
}

TEST(clear_and_set, strategy_assign) {
    using st = cas_strategy<reg_io::read_write, 0xFF, 0xF0, 0x0F>;
    auto type = st::strategy;
    EXPECT_EQ(type, cas_strategy_type::assign);

    using st2 = cas_strategy<reg_io::write, 0xFF, 0x00, 0x0F>;
    type = st2::strategy;
    EXPECT_EQ(type, cas_strategy_type::assign);
}

TEST(clear_and_set, memory_modification) {
    zoal::test::mem<void>::clear_mem();

    uint8_t *m = zoal::test::mem<void>::avr();
    cas<1, reg_io::read_write, uint8_t, 0xFF, 0, 0>();
    EXPECT_EQ(m[1], 0x00);

    cas<1, reg_io::read_write, uint8_t, 0xFF, 0, 1>();
    EXPECT_EQ(m[1], 0x01);

    cas<1, reg_io::read_write, uint8_t, 0xFF, 1, 0>();
    EXPECT_EQ(m[1], 0x00);

    cas<1, reg_io::read_write, uint8_t, 0xFF, 1, 1>();
    EXPECT_EQ(m[1], 0x01);

    cas<1, reg_io::read_write, uint8_t, 0xFF, 1, 3>();
    EXPECT_EQ(m[1], 0x03);

    cas<1, reg_io::write, uint8_t, 0xFF, 0, 0>();
    EXPECT_EQ(m[1], 0x03);

    cas<1, reg_io::write, uint8_t, 0xFF, 0, 4>();
    EXPECT_EQ(m[1], 0x04);

    cas<1, reg_io::read, uint8_t, 0xFF, 0, 0>();
    EXPECT_EQ(m[1], 0x04);
}

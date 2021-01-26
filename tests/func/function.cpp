#include "gtest/gtest.h"
#include <zoal/func/function.hpp>

using namespace zoal::func;

TEST(function, lambda_void_ret) {
    volatile int value = 0;
    function<64, void> f;

    EXPECT_FALSE(f);
    f.assign([&]() { value += 1; });
    EXPECT_TRUE(f);

    f();
    EXPECT_EQ(value, 1);

    f();
    EXPECT_EQ(value, 2);

    f.reset();
    EXPECT_FALSE(f);

    f = [&]() { value += 100; };
    EXPECT_TRUE(f);
    f();
    EXPECT_EQ(value, 102);
}

TEST(function, lambda_int_ret) {
    volatile int value = 0;
    function<64, int> f;

    EXPECT_FALSE(f);
    f.assign([&]() { value += 1; return value * 2;});
    EXPECT_TRUE(f);

    auto res = f();
    EXPECT_EQ(value, 1);
    EXPECT_EQ(res, 2);

    res = f();
    EXPECT_EQ(value, 2);
    EXPECT_EQ(res, 4);

    f.reset();
    EXPECT_FALSE(f);

    f = [&]() { value += 100; return value * 10;};
    EXPECT_TRUE(f);
    res = f();
    EXPECT_EQ(value, 102);
    EXPECT_EQ(res, 1020);
}

TEST(function, copy_ctor) {
    volatile int value = 0;
    function<64, void> f1;
    f1.assign([&]() { value += 1; });

    f1();
    EXPECT_EQ(value, 1);

    function<64, void> f2;
    function<64, void> &f2_ref = f2;
    EXPECT_FALSE(f2);

    f2 = f1;
    f2 = f2_ref;

    f2();
    EXPECT_EQ(value, 2);

    function<64, void> f3 = f1;
    EXPECT_TRUE(f3);

    f3();
    EXPECT_EQ(value, 3);
}
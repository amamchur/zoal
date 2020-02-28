#include "gtest/gtest.h"
#include "utils/accessor.hpp"

#include <zoal/ct/check.hpp>
#include <zoal/gpio/pin.hpp>
#include <zoal/mcu/atmega328p.hpp>

using namespace zoal::ct;
using namespace zoal::gpio;

TEST(StaticAssertion, static_type_checking) {
    auto a = is_same<int, int>::value;
    EXPECT_TRUE(a);

    auto b = is_same<int, void>::value;
    EXPECT_FALSE(b);
}

TEST(StaticAssertion, static_pin_checking) {
    using namespace ::zoal::gpio;

    auto a = belongs_to_port<null_pin, null_port>::value;
    EXPECT_TRUE(a);

    auto b = belongs_to_port<null_pin, zoal::mcu::atmega328p<16000000>>::value;
    EXPECT_FALSE(b);
}

TEST(StaticAssertion, static_list_checking) {
    using namespace ::zoal::gpio;

    auto a = has_same<int, float, char, double>::value;
    EXPECT_FALSE(a);

    auto b = has_same<int, float, char, double, int>::value;
    EXPECT_TRUE(b);

    auto c = has_same<int, int, float, char, double>::value;
    EXPECT_TRUE(c);

    auto d = has_same<float, char, double, int, int>::value;
    EXPECT_TRUE(d);

    auto e = has_same<float, char, int, int, double>::value;
    EXPECT_TRUE(e);

    auto f = has_same<double>::value;
    EXPECT_FALSE(f);
}

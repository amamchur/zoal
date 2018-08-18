#include "gtest/gtest.h"

#include <zoal/utils/helpers.hpp>

using namespace zoal::utils;

TEST(StaticAssertion, SameType) {  /* NOLINT */
    auto a = same_type<int, int>::value;
    EXPECT_TRUE(a);

    auto b = same_type<int, void>::value;
    EXPECT_FALSE(b);
}

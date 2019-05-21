#include "gtest/gtest.h"

#include <zoal/ct/check.hpp>

using namespace zoal::ct;

TEST(StaticAssertion, static_type_checking) {  /* NOLINT */
    auto a = is_same<int, int>::value;
    EXPECT_TRUE(a);

    auto b = is_same<int, void>::value;
    EXPECT_FALSE(b);
}

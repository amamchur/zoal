#include "gtest/gtest.h"

#include <zoal/utils/helpers.hpp>

using namespace zoal::utils;

TEST(StaticAssertion, static_type_checking) {  /* NOLINT */
    auto a = is_same<int, int>::value;
    EXPECT_TRUE(a);

    auto b = is_same<int, void>::value;
    EXPECT_FALSE(b);

    auto c = has_duplicates<int, void>::value;
    EXPECT_FALSE(c);

    auto d = has_duplicates<int, int>::value;
    EXPECT_TRUE(d);

    auto e = has_duplicates<int, void, float >::value;
    EXPECT_FALSE(e);

    auto f = has_duplicates<int, char, int>::value;
    EXPECT_TRUE(f);
}

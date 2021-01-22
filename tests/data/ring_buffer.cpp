#include "../test_utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <zoal/data/ring_buffer.hpp>

TEST(ring_buffer, push) {
    using buffer = zoal::data::ring_buffer<uint8_t, 3>;
    buffer rb;
    auto res = rb.push_back(1);
    EXPECT_TRUE(res);

    res = rb.push_back(2);
    EXPECT_TRUE(res);

    res = rb.push_back(3);
    EXPECT_TRUE(res);

    res = rb.push_back(4);
    EXPECT_FALSE(res);

    res = rb.push_back(5);
    EXPECT_FALSE(res);

    uint8_t value;
    res = rb.pop_front(value);
    EXPECT_TRUE(res);
    EXPECT_EQ(value, 1);

    res = rb.pop_front(value);
    EXPECT_TRUE(res);
    EXPECT_EQ(value, 2);

    res = rb.pop_front(value);
    EXPECT_TRUE(res);
    EXPECT_EQ(value, 3);

    res = rb.pop_front(value);
    EXPECT_FALSE(res);

    res = rb.pop_front(value);
    EXPECT_FALSE(res);

    res = rb.push_back(1);
    EXPECT_TRUE(res);

    res = rb.pop_front(value);
    EXPECT_TRUE(res);
    EXPECT_EQ(value, 1);
}

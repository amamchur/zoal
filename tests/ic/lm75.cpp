#include "../test_utils/i2c_mock.hpp"

#include "gtest/gtest.h"
#include <zoal/ic/lm75.hpp>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>

TEST(lm75, default_values) {
    auto lm75 = zoal::ic::lm75();
    auto temp = lm75.temperature();
    EXPECT_EQ(temp, 0.0f);
}

TEST(lm75, fetch_data_request) {
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;

    auto lm75 = zoal::ic::lm75();
    lm75.fetch(dispatcher)([&]() { finished = true; });

    EXPECT_FALSE(finished);
    EXPECT_EQ(request.address_rw(), 0x90);
    EXPECT_EQ(*request.ptr, 0);
    EXPECT_EQ(request.end - request.ptr, 1);

    dispatcher.handle();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();

    EXPECT_FALSE(finished);
    EXPECT_EQ(request.address_rw(), 0x91);
    EXPECT_EQ(*request.ptr, 0);
    EXPECT_EQ(request.end - request.ptr, 6);

    // Assign -25 degrees
    request.ptr[0] = 0xE7; // msb
    request.ptr[1] = 0x00; // lsb
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();

    auto temp = lm75.temperature();
    EXPECT_TRUE(finished);
    EXPECT_FLOAT_EQ(temp, -25.0f);
}

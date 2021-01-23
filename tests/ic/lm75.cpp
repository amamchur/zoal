#include "gtest/gtest.h"
#include <zoal/ic/lm75.hpp>
#include <zoal/periph/i2c_request.hpp>

TEST(lm75, default_values) {
    auto lm75 = zoal::ic::lm75();
    auto temp = lm75.temperature();
    EXPECT_EQ(temp, 0.0f);
}

TEST(lm75, fetch_data_request) {
    zoal::periph::i2c_request request;
    auto lm75 = zoal::ic::lm75();
    lm75.fetch(request);

    EXPECT_EQ(request.initiator, &lm75);
    EXPECT_EQ(request.address(), 0x90);
    EXPECT_EQ(*request.ptr, 0);
    EXPECT_EQ(request.end - request.ptr, 1);

    auto res = lm75.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    zoal::periph::i2c_request request2;
    res = lm75.complete_request(request2);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    request.result = zoal::periph::i2c_result::ok;
    res = lm75.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::new_request);

    EXPECT_EQ(request.initiator, &lm75);
    EXPECT_EQ(request.address(), 0x91);
    EXPECT_EQ(*request.ptr, 0);
    EXPECT_EQ(request.end - request.ptr, 6);

    request.result = zoal::periph::i2c_result::ok;
    // Assign -25 degrees
    request.ptr[0] = 0xE7; // msb
    request.ptr[1] = 0x00; // lsb
    res = lm75.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::finished);

    auto temp = lm75.temperature();
    EXPECT_FLOAT_EQ(temp, -25.0f);

    request.token = 0xDEADBEEF;
    res = lm75.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);
}

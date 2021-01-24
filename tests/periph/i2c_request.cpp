#include "gtest/gtest.h"
#include <vector>
#include <zoal/ic/lm75.hpp>
#include <zoal/periph/i2c_request.hpp>
#include "../test_utils/process_sync_i2c_mock.hpp"

using namespace zoal::periph;

TEST(i2c_request, default_values) {
    zoal::periph::i2c_request request;

    EXPECT_EQ(request.address_rw(), 0);
    EXPECT_FALSE(request.has_next());
    EXPECT_FALSE(request.request_next());
    EXPECT_FALSE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.initiator, nullptr);
    EXPECT_EQ(request.token, 0);
    EXPECT_EQ(request.context, nullptr);
    EXPECT_EQ(request.ptr, nullptr);
    EXPECT_EQ(request.end, nullptr);

    auto r = request.result;
    EXPECT_EQ(r, zoal::periph::i2c_result::idle);
}

TEST(i2c_request, write_data) {
    zoal::periph::i2c_request request;
    uint8_t buffer[] = {1, 2, 3, 5, 7};

    request.write(0x3C, buffer, buffer + sizeof(buffer));

    EXPECT_EQ(request.address_rw(), 0x78);
    EXPECT_TRUE(request.has_next());
    EXPECT_TRUE(request.request_next());
    EXPECT_TRUE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.initiator, nullptr);
    EXPECT_EQ(request.token, 0);
    EXPECT_EQ(request.context, nullptr);
    EXPECT_EQ(request.ptr, buffer);
    EXPECT_EQ(request.end, buffer + sizeof(buffer));

    auto r = request.result;
    EXPECT_EQ(r, zoal::periph::i2c_result::pending);

    std::vector<uint8_t> data;
    while (request.has_next()) {
        data.push_back(request.dequeue());
    }

    ASSERT_EQ(data.size(), 5);
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
    EXPECT_EQ(data[3], 5);
    EXPECT_EQ(data[4], 7);
}

TEST(i2c_request, read_data) {
    zoal::periph::i2c_request request;
    uint8_t buffer[10] = {0};

    request.read(0x3C, buffer, buffer + sizeof(buffer) / 2);
    EXPECT_EQ(request.address_rw(), 0x79);
    EXPECT_EQ(request.end - request.ptr, 5);
    EXPECT_TRUE(request.has_next());
    EXPECT_TRUE(request.request_next());
    EXPECT_TRUE(request.processing());
    EXPECT_FALSE(request.finished());

    uint8_t fib_prev = 1;
    uint8_t fib = 1;
    while (request.request_next()) {
        auto f = fib_prev + fib;
        request.enqueue(f);
        fib_prev = fib;
        fib = f;
    }

    EXPECT_EQ(buffer[0], 2);
    EXPECT_EQ(buffer[1], 3);
    EXPECT_EQ(buffer[2], 5);
    EXPECT_EQ(buffer[3], 8);
    EXPECT_EQ(buffer[4], 13);

    EXPECT_EQ(buffer[5], 0);
    EXPECT_EQ(buffer[6], 0);
    EXPECT_EQ(buffer[7], 0);
    EXPECT_EQ(buffer[8], 0);
    EXPECT_EQ(buffer[9], 0);
}

TEST(i2c_request, finished_state) {
    zoal::periph::i2c_request request;

    request.result = zoal::periph::i2c_result::idle;
    EXPECT_FALSE(request.finished());

    request.result = zoal::periph::i2c_result::pending;
    EXPECT_FALSE(request.finished());

    request.result = zoal::periph::i2c_result::ok;
    EXPECT_TRUE(request.finished());

    request.result = zoal::periph::i2c_result::end_of_stream;
    EXPECT_TRUE(request.finished());

    request.result = zoal::periph::i2c_result::error;
    EXPECT_TRUE(request.finished());
}

TEST(i2c_request, reset) {
    zoal::periph::i2c_request request;
    uint8_t buffer[] = {1, 2, 3, 5, 7};

    request.write(0x3C, buffer, buffer + sizeof(buffer));

    EXPECT_EQ(request.address_rw(), 0x78);
    EXPECT_TRUE(request.has_next());
    EXPECT_TRUE(request.request_next());
    EXPECT_TRUE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.initiator, nullptr);
    EXPECT_EQ(request.token, 0);
    EXPECT_EQ(request.context, nullptr);
    EXPECT_EQ(request.ptr, buffer);
    EXPECT_EQ(request.end, buffer + sizeof(buffer));

    request.reset();

    EXPECT_EQ(request.address_rw(), 0);
    EXPECT_FALSE(request.has_next());
    EXPECT_FALSE(request.request_next());
    EXPECT_FALSE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.initiator, nullptr);
    EXPECT_EQ(request.token, 0);
    EXPECT_EQ(request.context, nullptr);
    EXPECT_EQ(request.ptr, nullptr);
    EXPECT_EQ(request.end, nullptr);
}

TEST(i2c_request, process_sync) {
    zoal::ic::lm75 lm75;
    using i2c_mock = zoal::tests::process_sync_i2c_mock<typeof(this)>;

    auto &request = i2c_mock::request;
    lm75.fetch(request);

    auto result = process_i2c_request_sync<i2c_mock>(request, lm75);
    EXPECT_EQ(result, i2c_request_completion_result::finished);

    auto temp = lm75.temperature();
    EXPECT_FLOAT_EQ(temp, -25.0f);

    i2c_mock::join();

    request.reset();
    result = process_i2c_request_sync<i2c_mock>(request, lm75);
    EXPECT_EQ(result, i2c_request_completion_result::ignored);

    temp = lm75.temperature();
    EXPECT_FLOAT_EQ(temp, -25.0f);

    i2c_mock::join();
}

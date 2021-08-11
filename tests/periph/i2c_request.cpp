#include "gtest/gtest.h"
#include <vector>
#include <zoal/ic/lm75.hpp>
#include <zoal/periph/i2c_request.hpp>
#include "../test_utils/process_sync_i2c_mock.hpp"

using namespace zoal::periph;

TEST(i2c_request, default_values) {
    zoal::periph::i2c_request request;

    EXPECT_EQ(request.address_rw(), 0);
    EXPECT_TRUE(request.eos());
    EXPECT_FALSE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.ptr, nullptr);
    EXPECT_EQ(request.end, nullptr);

    auto r = request.status;
    EXPECT_EQ(r, zoal::periph::i2c_request_status::idle);
}

TEST(i2c_request, write_data) {
    zoal::periph::i2c_request request;
    uint8_t buffer[] = {1, 2, 3, 5, 7};

    request.write(0x3C, buffer, buffer + sizeof(buffer));

    EXPECT_EQ(request.address_rw(), 0x78);
    EXPECT_FALSE(request.eos());
    EXPECT_TRUE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.ptr, buffer);
    EXPECT_EQ(request.end, buffer + sizeof(buffer));

    auto r = request.status;
    EXPECT_EQ(r, zoal::periph::i2c_request_status::pending);

    std::vector<uint8_t> data;
    while (!request.eos()) {
        data.push_back(request.pull());
    }
    EXPECT_EQ(request.pull(), 0);

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
    EXPECT_FALSE(request.eos());
    EXPECT_TRUE(request.processing());
    EXPECT_FALSE(request.finished());

    uint8_t fib_prev = 1;
    uint8_t fib = 1;
    while (!request.eos()) {
        auto f = fib_prev + fib;
        request.push(f);
        fib_prev = fib;
        fib = f;
    }

    request.push(0xFF); // Must be ignored
    request.push(0xFF);

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

    request.status = zoal::periph::i2c_request_status::idle;
    EXPECT_FALSE(request.finished());

    request.status = zoal::periph::i2c_request_status::pending;
    EXPECT_FALSE(request.finished());

    request.status = zoal::periph::i2c_request_status::finished;
    EXPECT_TRUE(request.finished());

    request.status = zoal::periph::i2c_request_status::finished_eos;
    EXPECT_TRUE(request.finished());

    request.status = zoal::periph::i2c_request_status::failed;
    EXPECT_TRUE(request.finished());
}

TEST(i2c_request, reset) {
    zoal::periph::i2c_request request;
    uint8_t buffer[] = {1, 2, 3, 5, 7};

    request.write(0x3C, buffer, buffer + sizeof(buffer));

    EXPECT_EQ(request.address_rw(), 0x78);
    EXPECT_FALSE(request.eos());
    EXPECT_TRUE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.ptr, buffer);
    EXPECT_EQ(request.end, buffer + sizeof(buffer));

    request.reset();

    EXPECT_EQ(request.address_rw(), 0);
    EXPECT_TRUE(request.eos());
    EXPECT_FALSE(request.processing());
    EXPECT_FALSE(request.finished());

    EXPECT_EQ(request.ptr, nullptr);
    EXPECT_EQ(request.end, nullptr);
}


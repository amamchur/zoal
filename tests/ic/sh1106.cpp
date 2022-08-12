#include "../test_utils/delay.hpp"
#include "../test_utils/i2c_mock.hpp"
#include "../test_utils/ms_counter.hpp"

#include "gtest/gtest.h"
#include <vector>
#include <zoal/gpio/pin.hpp>
#include <zoal/ic/sh1106.hpp>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>

TEST(sh1106, display) {
    using sh1106_type = zoal::ic::sh1106_i2c<128, 64, 0x3C>;
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;
    sh1106_type display;

    memset(display.buffer.control_buffer, 0, sizeof(display.buffer.control_buffer));
    memset(display.buffer.canvas, 0, sizeof(display.buffer.canvas));

    display.buffer.canvas[0] = 1;
    display.buffer.canvas[1] = 2;
    display.buffer.canvas[2] = 3;
    display.buffer.canvas[3] = 4;

    display.display(dispatcher)([&finished](int code) { finished = code == 0; });

    auto offset = request.ptr - display.buffer.control_buffer;
    // Canvas corruption checking
    EXPECT_GE(offset, 0);
    EXPECT_EQ(display.buffer.canvas[0], 1);
    EXPECT_EQ(display.buffer.canvas[1], 2);
    EXPECT_EQ(display.buffer.canvas[2], 3);
    EXPECT_EQ(display.buffer.canvas[3], 4);

    std::vector<uint8_t> vect;
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB0); // Page 0
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);
    EXPECT_EQ(vect[4], 0x80);
    EXPECT_EQ(vect[5], 0x10);
    EXPECT_EQ(vect[6], 0x40);
    EXPECT_EQ(vect[7], 0x01);
    EXPECT_EQ(vect[8], 0x02);
    EXPECT_EQ(vect[9], 0x03);
    EXPECT_EQ(vect[10], 0x04);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB1); // Page 1
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB2); // Page 2
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB3); // Page 3
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB4); // Page 4
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB5); // Page 5
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB6); // Page 6
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_FALSE(finished);
    ASSERT_EQ(vect.size(), 135);
    EXPECT_EQ(vect[0], 0x80);
    EXPECT_EQ(vect[1], 0xB7); // Page 7
    EXPECT_EQ(vect[2], 0x80);
    EXPECT_EQ(vect[3], 0x02);

    vect.clear();
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    while (!request.eos()) {
        vect.push_back(request.pull());
    }

    EXPECT_TRUE(finished);
    ASSERT_EQ(vect.size(), 0);
}

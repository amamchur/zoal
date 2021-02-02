#include "../test_utils/delay.hpp"
#include "../test_utils/i2c_mock.hpp"
#include "../test_utils/ms_counter.hpp"

#include "gtest/gtest.h"
#include <zoal/gpio/pin.hpp>
#include <zoal/ic/ssd1306.hpp>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>

TEST(ssd1306, init_oled) {
    using counter = zoal::tests::ms_counter;
    using delay = zoal::tests::delay<counter>;
    using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<delay, zoal::gpio::null_pin, zoal::gpio::null_pin, 0x3C>;
    using ssd1306_type = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;
    ssd1306_type display;

    display.init(dispatcher)([&finished](int code) {
        finished = code == 0;
    });

    EXPECT_EQ(request.address_rw(), 0x3C << 1);

    auto size = request.end - request.ptr;
    auto ptr = request.ptr;
    ASSERT_EQ(size, 50);

    EXPECT_EQ(ptr[0], 128);
    EXPECT_EQ(ptr[1], 174);
    EXPECT_EQ(ptr[2], 128);
    EXPECT_EQ(ptr[3], 213);
    EXPECT_EQ(ptr[4], 128);
    EXPECT_EQ(ptr[5], 128);
    EXPECT_EQ(ptr[6], 128);
    EXPECT_EQ(ptr[7], 168);
    EXPECT_EQ(ptr[8], 128);
    EXPECT_EQ(ptr[9], 63);

    EXPECT_EQ(ptr[10], 128);
    EXPECT_EQ(ptr[11], 211);
    EXPECT_EQ(ptr[12], 128);
    EXPECT_EQ(ptr[13], 0);
    EXPECT_EQ(ptr[14], 128);
    EXPECT_EQ(ptr[15], 64);
    EXPECT_EQ(ptr[16], 128);
    EXPECT_EQ(ptr[17], 141);
    EXPECT_EQ(ptr[18], 128);
    EXPECT_EQ(ptr[19], 20);

    // ...

    EXPECT_FALSE(finished);
    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    EXPECT_TRUE(finished);
}

TEST(ssd1306, display) {
    using counter = zoal::tests::ms_counter;
    using delay = zoal::tests::delay<counter>;
    using ssd1306_interface = zoal::ic::ssd1306_interface_i2c<delay, zoal::gpio::null_pin, zoal::gpio::null_pin, 0x3C>;
    using ssd1306_type = zoal::ic::ssd1306<zoal::ic::ssd1306_resolution::ssd1306_128x64, ssd1306_interface>;
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;
    ssd1306_type display;

    memset(display.buffer.control_buffer, 0, sizeof(display.buffer.control_buffer));
    memset(display.buffer.canvas, 0, sizeof(display.buffer.canvas));

    display.buffer.canvas[0] = 1;
    display.buffer.canvas[1] = 2;
    display.buffer.canvas[2] = 3;
    display.buffer.canvas[3] = 4;

    display.display(dispatcher)([&finished](int code) {
        finished = code == 0;
    });

    auto offset = request.ptr - display.buffer.control_buffer;
    // Canvas corruption checking
    EXPECT_GE(offset, 0);
    EXPECT_EQ(display.buffer.canvas[0], 1);
    EXPECT_EQ(display.buffer.canvas[1], 2);
    EXPECT_EQ(display.buffer.canvas[2], 3);
    EXPECT_EQ(display.buffer.canvas[3], 4);

    auto ptr = request.ptr;
    EXPECT_EQ(ptr[0], 0x80);
    EXPECT_EQ(ptr[1], 0x21);
    EXPECT_EQ(ptr[2], 0x80);
    EXPECT_EQ(ptr[3], 0x00);
    EXPECT_EQ(ptr[4], 0x80);
    EXPECT_EQ(ptr[5], 0x7F);
    EXPECT_EQ(ptr[6], 0x80);
    EXPECT_EQ(ptr[7], 0x22);
    EXPECT_EQ(ptr[8], 0x80);
    EXPECT_EQ(ptr[9], 0x00);
    EXPECT_EQ(ptr[10], 0x80);
    EXPECT_EQ(ptr[11], 0x07);
    EXPECT_EQ(ptr[12], 0x40); // Data start control byte

    // Canvas start
    EXPECT_EQ(ptr + 13, display.buffer.canvas);
    EXPECT_EQ(ptr[13], 0x01);
    EXPECT_EQ(ptr[14], 0x02);
    EXPECT_EQ(ptr[15], 0x03);
    EXPECT_EQ(ptr[16], 0x04);
}

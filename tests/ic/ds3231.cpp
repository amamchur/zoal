#include "../test_utils/i2c_mock.hpp"

#include "gtest/gtest.h"
#include <zoal/ic/ds3231.hpp>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>

TEST(ds3231, default_values) {
    auto ds3231 = zoal::ic::ds3231();
    auto dt = ds3231.date_time();
    EXPECT_EQ(dt.year, 2000);
    EXPECT_EQ(dt.month, 0);
    EXPECT_EQ(dt.date, 0);
    EXPECT_EQ(dt.day, 0);
    EXPECT_EQ(dt.hours, 0);
    EXPECT_EQ(dt.minutes, 0);
    EXPECT_EQ(dt.seconds, 0);

    auto temp = ds3231.temperature();
    EXPECT_EQ(temp, 0.0f);
}

TEST(ds3231, date_time_to_regs_21xx) {
    using ra = zoal::ic::ds3231::register_address;
    auto ds3231 = zoal::ic::ds3231();
    zoal::data::date_time dt;
    dt.year = 2121;
    dt.month = 1;
    dt.date = 23;
    dt.day = zoal::data::saturday;
    dt.hours = 15;
    dt.minutes = 27;
    dt.seconds = 30;

    ds3231.date_time(dt);

    // Values in BCD format
    EXPECT_EQ(ds3231[ra::seconds], 0x30);
    EXPECT_EQ(ds3231[ra::minutes], 0x27);
    EXPECT_EQ(ds3231[ra::hours], 0x15);
    EXPECT_EQ(ds3231[ra::day], 0x06);
    EXPECT_EQ(ds3231[ra::date], 0x23);
    EXPECT_EQ(ds3231[ra::month_century], 0x81);
    EXPECT_EQ(ds3231[ra::year], 0x21);

    auto dt2 = ds3231.date_time();

    EXPECT_EQ(dt.seconds, dt2.seconds);
    EXPECT_EQ(dt.minutes, dt2.minutes);
    EXPECT_EQ(dt.hours, dt2.hours);
    EXPECT_EQ(dt.day, dt2.day);
    EXPECT_EQ(dt.date, dt2.date);
    EXPECT_EQ(dt.month, dt2.month);
    EXPECT_EQ(dt.year, dt2.year);
}

TEST(ds3231, date_time_to_regs_20xx) {
    using ra = zoal::ic::ds3231::register_address;
    auto ds3231 = zoal::ic::ds3231();
    zoal::data::date_time dt;
    dt.year = 2021;
    dt.month = 1;
    dt.date = 23;
    dt.day = zoal::data::sunday;
    dt.hours = 15;
    dt.minutes = 27;
    dt.seconds = 30;

    ds3231.date_time(dt);

    // Values in BCD format
    EXPECT_EQ(ds3231[ra::seconds], 0x30);
    EXPECT_EQ(ds3231[ra::minutes], 0x27);
    EXPECT_EQ(ds3231[ra::hours], 0x15);
    EXPECT_EQ(ds3231[ra::day], 0x07);
    EXPECT_EQ(ds3231[ra::date], 0x23);
    EXPECT_EQ(ds3231[ra::month_century], 0x01);
    EXPECT_EQ(ds3231[ra::year], 0x21);

    auto dt2 = ds3231.date_time();

    EXPECT_EQ(dt.seconds, dt2.seconds);
    EXPECT_EQ(dt.minutes, dt2.minutes);
    EXPECT_EQ(dt.hours, dt2.hours);
    EXPECT_EQ(dt.day, dt2.day);
    EXPECT_EQ(dt.date, dt2.date);
    EXPECT_EQ(dt.month, dt2.month);
    EXPECT_EQ(dt.year, dt2.year);
}

TEST(ds3231, fetch_data_request) {
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;

    using ra = zoal::ic::ds3231::register_address;
    auto ds3231 = zoal::ic::ds3231();
    ds3231.fetch(dispatcher)([&]() { finished = true; });

    EXPECT_FALSE(finished);
    EXPECT_EQ(request.address_rw(), 0xD0);
    EXPECT_EQ(*request.ptr, static_cast<uint8_t>(ra::seconds));
    EXPECT_EQ(request.end - request.ptr, 1);

    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();

    EXPECT_FALSE(finished);
    EXPECT_EQ(request.address_rw(), 0xD1);
    EXPECT_EQ(*request.ptr, static_cast<uint8_t>(ra::seconds));
    EXPECT_EQ(request.end - request.ptr, 19);

    uint8_t &ref = ds3231[ra::seconds];
    uint8_t *ptr = &ref;
    EXPECT_EQ(request.ptr, ptr);

    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    EXPECT_TRUE(finished);
}

TEST(ds3231, update_data_request) {
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;

    using ra = zoal::ic::ds3231::register_address;
    auto ds3231 = zoal::ic::ds3231();
    zoal::data::date_time dt;
    dt.year = 2021;
    dt.month = 1;
    dt.date = 23;
    dt.day = zoal::data::saturday;
    dt.hours = 15;
    dt.minutes = 27;
    dt.seconds = 30;

    ds3231.date_time(dt);
    ds3231.update(dispatcher)([&]() { finished = true; });

    uint8_t &ref = ds3231[ra::seconds];
    uint8_t *ptr = &ref;
    EXPECT_FALSE(finished);
    EXPECT_EQ(request.address_rw(), 0xD0);
    EXPECT_EQ(request.ptr, ptr - 1);
    EXPECT_EQ(request.end - request.ptr, 19);

    request.status = zoal::periph::i2c_request_status::finished;
    dispatcher.handle();
    EXPECT_TRUE(finished);
}

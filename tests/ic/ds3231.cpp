#include "gtest/gtest.h"
#include <zoal/ic/ds3231.hpp>
#include <zoal/periph/i2c_request.hpp>

TEST(ds3231, default_values) {
    auto ds3231 = zoal::ic::ds3231();
    auto dt = ds3231.date_time();
    EXPECT_EQ(dt.year, 1900);
    EXPECT_EQ(dt.month, 0);
    EXPECT_EQ(dt.date, 0);
    EXPECT_EQ(dt.day, 0);
    EXPECT_EQ(dt.hours, 0);
    EXPECT_EQ(dt.minutes, 0);
    EXPECT_EQ(dt.seconds, 0);

    auto temp = ds3231.temperature();
    EXPECT_EQ(temp, 0.0f);
}

TEST(ds3231, date_time_to_regs_20xx) {
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

TEST(ds3231, date_time_to_regs_19xx) {
    using ra = zoal::ic::ds3231::register_address;
    auto ds3231 = zoal::ic::ds3231();
    zoal::data::date_time dt;
    dt.year = 1921;
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
    using ra = zoal::ic::ds3231::register_address;
    zoal::periph::i2c_request request;
    auto ds3231 = zoal::ic::ds3231();
    ds3231.fetch(request);

    EXPECT_EQ(request.initiator, &ds3231);
    EXPECT_EQ(request.address_rw(), 0xD0);
    EXPECT_EQ(*request.ptr, static_cast<uint8_t>(ra::seconds));
    EXPECT_EQ(request.end - request.ptr, 1);

    auto res = ds3231.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    zoal::periph::i2c_request request2;
    res = ds3231.complete_request(request2);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    request.result = zoal::periph::i2c_result::ok;
    res = ds3231.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::new_request);

    EXPECT_EQ(request.initiator, &ds3231);
    EXPECT_EQ(request.address_rw(), 0xD1);
    EXPECT_EQ(*request.ptr, static_cast<uint8_t>(ra::seconds));
    EXPECT_EQ(request.end - request.ptr, 19);

    uint8_t &ref = ds3231[ra::seconds];
    uint8_t *ptr = &ref;
    EXPECT_EQ(request.ptr, ptr);

    request.result = zoal::periph::i2c_result::ok;
    res = ds3231.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::finished);

    request.token = 0xDEADBEEF;
    res = ds3231.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);
}

TEST(ds3231, update_data_request) {
    using ra = zoal::ic::ds3231::register_address;
    zoal::periph::i2c_request request;
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
    ds3231.update(request);

    uint8_t &ref = ds3231[ra::seconds];
    uint8_t *ptr = &ref;
    EXPECT_EQ(request.initiator, &ds3231);
    EXPECT_EQ(request.address_rw(), 0xD0);
    EXPECT_EQ(request.ptr, ptr);
    EXPECT_EQ(request.end - request.ptr, 19);

    request.result = zoal::periph::i2c_result::ok;
    auto res = ds3231.complete_request(request);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::finished);
}

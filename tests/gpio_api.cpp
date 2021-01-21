#include "./utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <sstream>
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/mcu/atmega328p.hpp>

using namespace zoal::gpio;
using namespace zoal::ct;
using mcu = zoal::mcu::atmega328p;

TEST(gpio_api, optimize_cas_ingore_duplicates) {
    using cas_list = api::optimize<api::mode<zoal::gpio::pin_mode::output,
        //
        mcu::pb_00,
        mcu::pb_00,
        mcu::pb_00,
        mcu::pb_00,
        mcu::pb_00,
        mcu::pb_00,
        mcu::pb_00,
        mcu::pb_00>>;
    auto count = cas_list::count;
    EXPECT_EQ(count, 1);

    cas_collector c;
    type_list_iterator<cas_list>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0x24, 0x00, 0x01));
}

TEST(gpio_api, optimize_cas_aggregate) {
    using cas_list = api::optimize<api::mode<zoal::gpio::pin_mode::output,
                                             //
                                             mcu::pb_00,
                                             mcu::pb_01,
                                             mcu::pb_02,
                                             mcu::pb_03,
                                             mcu::pb_04,
                                             mcu::pb_05,
                                             mcu::pb_06,
                                             mcu::pb_07>>;
    auto count = cas_list::count;
    EXPECT_EQ(count, 1);

    cas_collector c;
    type_list_iterator<cas_list>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0x24, 0x00, 0xff));
}

TEST(gpio_api, optimize_cas_keep_order_1) {
    using cas_list = api::optimize<api::mode<zoal::gpio::pin_mode::output, mcu::pb_00, mcu::pc_01>>;
    auto count = cas_list::count;
    EXPECT_EQ(count, 2);

    cas_collector c;
    type_list_iterator<cas_list>::for_each(c);

    EXPECT_EQ(c.values.size(), 2);
    EXPECT_EQ(c.values[0], cas_value(0x24, 0x00, 0x01));
    EXPECT_EQ(c.values[1], cas_value(0x27, 0x00, 0x02));
}

TEST(gpio_api, optimize_cas_keep_order_2) {
    using cas_list = api::optimize<api::mode<zoal::gpio::pin_mode::output, mcu::pc_01, mcu::pb_00>>;
    auto count = cas_list::count;
    EXPECT_EQ(count, 2);

    cas_collector c;
    type_list_iterator<cas_list>::for_each(c);

    EXPECT_EQ(c.values.size(), 2);
    EXPECT_EQ(c.values[0], cas_value(0x27, 0x00, 0x02));
    EXPECT_EQ(c.values[1], cas_value(0x24, 0x00, 0x01));
}

#include "../../../test_utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/mcu/atmega328p.hpp>

TEST(avr_atmega_port, output_mode) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    {
        using gpio_cfg_1 = api::optimize<mcu::port_b::mode_cas<pin_mode::output, 0x3>>;
        using gpio_cfg_2 = api::optimize<api::mode<pin_mode::output, mcu::pb_00, mcu::pb_01>>;
        cas_collector c1, c2;
        type_list_iterator<gpio_cfg_1>::for_each(c1);
        type_list_iterator<gpio_cfg_2>::for_each(c2);
        EXPECT_EQ(c1, c2);
    }
    {
        // output == output_push_pull in AVR
        using gpio_cfg_1 = api::optimize<mcu::port_b::mode_cas<pin_mode::output_push_pull, 0x3>>;
        using gpio_cfg_2 = api::optimize<mcu::port_b::mode_cas<pin_mode::output, 0x3>>;
        cas_collector c1, c2;
        type_list_iterator<gpio_cfg_1>::for_each(c1);
        type_list_iterator<gpio_cfg_2>::for_each(c2);
        EXPECT_EQ(c1, c2);
    }
    {
        using gpio_cfg_1 = api::optimize<api::mode<pin_mode::output, mcu::pb_00, mcu::pb_01>>;
        cas_collector c;
        type_list_iterator<gpio_cfg_1>::for_each(c);
        EXPECT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x24, 0x00, 0x03, sizeof(uint8_t)));
    }
    {
        using gpio_cfg_1 = api::optimize<api::mode<
            //
            pin_mode::output,
            mcu::pb_00,
            mcu::pb_01,
            mcu::pb_02,
            mcu::pb_03,
            mcu::pb_04,
            mcu::pb_05,
            mcu::pb_06,
            mcu::pb_07>>;
        cas_collector c;
        type_list_iterator<gpio_cfg_1>::for_each(c);
        EXPECT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x24, 0x00, 0xFF, sizeof(uint8_t)));
    }
}

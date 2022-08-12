#include "../../../test_utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/periph/i2c.hpp>

TEST(stm32f1_i2c, enabled_disable) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::stm32f103c8tx;
    {
        cas_collector c;
        type_list_iterator<mcu::i2c_01::enable_cas>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x40005400, 0x00000000, 0x00000001, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<mcu::i2c_01::disable_cas>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x40005400, 0x00000001, 0x00000000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<mcu::i2c_01::clock_on_cas>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x4002101c, 0x00200000, 0x00200000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<mcu::i2c_01::clock_off_cas>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x4002101c, 0x00200000, 0x00000000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<mcu::i2c_02::clock_on_cas>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x4002101c, 0x00400000, 0x00400000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<mcu::i2c_02::clock_off_cas>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x4002101c, 0x00400000, 0x00000000, sizeof(uint32_t)));
    }
}

TEST(stm32f1_i2c, i2c_fasm_mode_duty_0) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::stm32f103c8tx;
    using i2c = mcu::i2c_02;
    using i2c_02_cfg = zoal::periph::i2c_params<36000000, 400000>;
    using i2c_cfg = mcu::cfg::i2c<i2c, i2c_02_cfg>;

    {
        auto value = i2c_cfg::max_rise_time_standard_mode_ns;
        EXPECT_EQ(value, 1000);
    }
    {
        auto value = i2c_cfg::max_rise_time_fast_mode_ns;
        EXPECT_EQ(value, 300);
    }
    {
        auto value = i2c_cfg::peripheral_clock;
        EXPECT_EQ(value, 36000000);
    }
    {
        auto value = i2c_cfg::peripheral_clock_mhz;
        EXPECT_EQ(value, 36);
    }
    {
        auto value = i2c_cfg::i2c_frequency;
        EXPECT_EQ(value, 400000);
    }
    {
        auto value = i2c_cfg::i2c_standard_mode;
        EXPECT_FALSE(value);
    }
    {
        auto value = i2c_cfg::max_rise_time;
        EXPECT_EQ(value, 11);
    }
    {
        auto value = i2c_cfg::standard_mode_ccr;
        EXPECT_EQ(value, 45);
    }

    {
        auto value = i2c_cfg::fast_mode_duty_0_ccr;
        EXPECT_EQ(value, 30);
    }
    {
        auto value = i2c_cfg::fast_mode_duty_1_ccr;
        EXPECT_EQ(value, 3);
    }
    {
        auto value = i2c_cfg::duty;
        EXPECT_EQ(value, 0);
    }
    {
        auto value = i2c_cfg::ccr;
        EXPECT_EQ(value, 30);
    }
    {
        cas_collector c;
        type_list_iterator<i2c_cfg::apply>::for_each(c);
        ASSERT_EQ(c.values.size(), 6);
        EXPECT_EQ(c.values[0], cas_value(0x40005800, 0x000000c0, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40005804, 0x0000003f, 0x00000024, sizeof(uint32_t)));
        EXPECT_EQ(c.values[2], cas_value(0x40005808, 0xffffffff, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[3], cas_value(0x4000580c, 0xffffffff, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[4], cas_value(0x4000581c, 0xffffffff, 0x0000801e, sizeof(uint32_t)));
        EXPECT_EQ(c.values[5], cas_value(0x40005820, 0x0000003f, 0x0000000b, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<i2c_cfg::clock_on>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x4002101c, 0x00400000, 0x00400000, sizeof(uint32_t)));
    }
}

TEST(stm32f1_i2c, i2c_fasm_mode_duty_1) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::stm32f103c8tx;
    using i2c = mcu::i2c_02;
    using i2c_02_cfg = zoal::periph::i2c_params<40000000, 400000>;
    using i2c_cfg = mcu::cfg::i2c<i2c, i2c_02_cfg>;

    {
        auto value = i2c_cfg::max_rise_time_standard_mode_ns;
        EXPECT_EQ(value, 1000);
    }
    {
        auto value = i2c_cfg::max_rise_time_fast_mode_ns;
        EXPECT_EQ(value, 300);
    }
    {
        auto value = i2c_cfg::peripheral_clock;
        EXPECT_EQ(value, 40000000);
    }
    {
        auto value = i2c_cfg::peripheral_clock_mhz;
        EXPECT_EQ(value, 40);
    }
    {
        auto value = i2c_cfg::i2c_frequency;
        EXPECT_EQ(value, 400000);
    }
    {
        auto value = i2c_cfg::i2c_standard_mode;
        EXPECT_FALSE(value);
    }
    {
        auto value = i2c_cfg::max_rise_time;
        EXPECT_EQ(value, 13);
    }
    {
        auto value = i2c_cfg::standard_mode_ccr;
        EXPECT_EQ(value, 50);
    }

    {
        auto value = i2c_cfg::fast_mode_duty_0_ccr;
        EXPECT_EQ(value, 33);
    }
    {
        auto value = i2c_cfg::fast_mode_duty_1_ccr;
        EXPECT_EQ(value, 4);
    }
    {
        auto value = i2c_cfg::duty;
        EXPECT_EQ(value, 1);
    }
    {
        auto value = i2c_cfg::ccr;
        EXPECT_EQ(value, 4);
    }
    {
        cas_collector c;
        type_list_iterator<i2c_cfg::apply>::for_each(c);
        ASSERT_EQ(c.values.size(), 6);
        EXPECT_EQ(c.values[0], cas_value(0x40005800, 0x000000c0, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40005804, 0x0000003f, 0x00000028, sizeof(uint32_t)));
        EXPECT_EQ(c.values[2], cas_value(0x40005808, 0xffffffff, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[3], cas_value(0x4000580c, 0xffffffff, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[4], cas_value(0x4000581c, 0xffffffff, 0x0000c004, sizeof(uint32_t)));
        EXPECT_EQ(c.values[5], cas_value(0x40005820, 0x0000003f, 0x0000000d, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<i2c_cfg::clock_on>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x4002101c, 0x00400000, 0x00400000, sizeof(uint32_t)));
    }
}

TEST(stm32f1_i2c, mux) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::stm32f103c8tx;
    using i2c = mcu::i2c_02;
    using i2c_mux = mcu::mux::i2c<i2c, mcu::pb_11, mcu::pb_10>;

    {
        cas_collector c;
        type_list_iterator<i2c_mux::connect>::for_each(c);
        ASSERT_EQ(c.values.size(), 2);
        EXPECT_EQ(c.values[0], cas_value(0x40010c00, 0x00000000, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40010c04, 0x0000ff00, 0x0000ff00, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<i2c_mux::disconnect>::for_each(c);
        ASSERT_EQ(c.values.size(), 3);
        EXPECT_EQ(c.values[0], cas_value(0x40010c00, 0x00000000, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40010c04, 0x0000ff00, 0x00004400, sizeof(uint32_t)));
        EXPECT_EQ(c.values[2], cas_value(0x40010c10, 0x00000000, 0x00000000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<i2c_mux::clock_on>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x40021018, 0x00000008, 0x00000008, sizeof(uint32_t)));
    }
}

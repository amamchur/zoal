#include "../../../test_utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/periph/usart.hpp>

TEST(stm32f1_usart, speed_115200) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::stm32f103c8tx;
    using usart = mcu::usart_01;
    using usart_cfg = zoal::periph::usart_115200<72000000>;
    using usart_config = mcu::cfg::usart<usart, usart_cfg>;
    {
        cas_collector c;
        type_list_iterator<usart_config::apply>::for_each(c);
        ASSERT_EQ(c.values.size(), 4);
        EXPECT_EQ(c.values[0], cas_value(0x4001380c, 0x10001600, 0x0000000c, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40013810, 0x00003000, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[2], cas_value(0x40013814, 0x00000300, 0x00000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[3], cas_value(0x40013808, 0x00000000, 0x00000271, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<usart_config::periph_clock_on>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x40021018, 0x00004000, 0x00004000, sizeof(uint32_t)));
    }
}

TEST(stm32f1_usart, mux) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::stm32f103c8tx;
    using usart = mcu::usart_01;
    using usart_mux = mcu::mux::usart<usart, mcu::pb_07, mcu::pb_06>;
    {
        cas_collector c;
        type_list_iterator<usart_mux::connect>::for_each(c);
        ASSERT_EQ(c.values.size(), 2);
        EXPECT_EQ(c.values[0], cas_value(0x40010004, 0x00000004, 0x00000004, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40010c00, 0xff000000, 0x4b000000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<usart_mux::disconnect>::for_each(c);
        ASSERT_EQ(c.values.size(), 2);
        EXPECT_EQ(c.values[0], cas_value(0x40010c00, 0xff000000, 0x44000000, sizeof(uint32_t)));
        EXPECT_EQ(c.values[1], cas_value(0x40010004, 0x00000004, 0x00000000, sizeof(uint32_t)));
    }
    {
        cas_collector c;
        type_list_iterator<usart_mux::periph_clock_on>::for_each(c);
        ASSERT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x40021018, 0x00000008, 0x00000008, sizeof(uint32_t)));
    }
}

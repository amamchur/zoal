#include "../../../test_utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/mcu/atmega328p.hpp>

TEST(avr_atmega_adc, mux) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using adc = mcu::adc_00;
    using gpio_cfg = api::optimize<mcu::mux::adc<adc, mcu::pc_00>::connect>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 2);
    EXPECT_EQ(c.values[0], cas_value(0x7C, 0x0F, 0x00, sizeof(uint8_t)));
    EXPECT_EQ(c.values[1], cas_value(0x7B, 0x08, 0x00, sizeof(uint8_t)));
}

TEST(avr_atmega_adc, cfg) {
    using namespace zoal::gpio;
    using namespace zoal::periph;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using adc = mcu::adc_00;
    using gpio_cfg = api::optimize<mcu::cfg::adc<adc, adc_config<>>::apply>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 2);
    EXPECT_EQ(c.values[0], cas_value(0x7A, 0x07, 0x07, sizeof(uint8_t)));
    EXPECT_EQ(c.values[1], cas_value(0x7C, 0xC0, 0x40, sizeof(uint8_t)));
}

TEST(avr_atmega_adc, enable) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using adc = mcu::adc_00;
    using gpio_cfg = api::optimize<api::enable<adc>>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0x7A, 0x00, 0x80, sizeof(uint8_t)));
}

TEST(avr_atmega_adc, disable) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using adc = mcu::adc_00;
    using gpio_cfg = api::optimize<api::disable<adc>>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0x7A, 0x80, 0x00, sizeof(uint8_t)));
}
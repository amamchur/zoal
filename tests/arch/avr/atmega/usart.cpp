#include "../../../test_utils/cas_collector.hpp"

#include "gtest/gtest.h"
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/mcu/atmega328p.hpp>
#include <zoal/mcu/atmega32u4.hpp>
#include <zoal/mcu/atmega2560.hpp>

TEST(avr_atmega328p_usart, mux) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using usart = mcu::usart_00;
    using gpio_cfg = api::optimize<mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::connect>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // No changes, just compile time checking
}

TEST(avr_atmega328p_usart, cfg_115200_at_16MHz) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using usart = mcu::usart_00;
    using gpio_cfg = api::optimize<mcu::cfg::usart<usart, zoal::periph::usart_115200<16000000>>::apply>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 4);
    EXPECT_EQ(c.values[0], cas_value(0xC4, 0xFF, 0x10, sizeof(uint8_t))); // UBRR0L
    EXPECT_EQ(c.values[1], cas_value(0xC5, 0xFF, 0x00, sizeof(uint8_t))); // UBRR0H
    EXPECT_EQ(c.values[2], cas_value(0xC0, 0xFF, 0x02, sizeof(uint8_t))); // UCSR0A
    EXPECT_EQ(c.values[3], cas_value(0xC2, 0xFF, 0x06, sizeof(uint8_t))); // UCSR0C
}

TEST(avr_atmega328p_usart, enable) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using usart = mcu::usart_00;
    using gpio_cfg = api::optimize<api::enable<usart>>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0xC1, 0x00, 0x98, sizeof(uint8_t)));
}

TEST(avr_atmega328p_usart, disable) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega328p;
    using usart = mcu::usart_00;
    using gpio_cfg = api::optimize<api::disable<usart>>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0xC1, 0x98, 0x00, sizeof(uint8_t)));
}

TEST(avr_atmega32u4_usart, mux) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega32u4;
    using usart = mcu::usart_01;
    using gpio_cfg = api::optimize<mcu::mux::usart<usart, mcu::pd_02, mcu::pd_03>::connect>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 1);
    EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // No changes, just compile time checking
}

TEST(avr_atmega32u4_usart, cfg_115200_at_16MHz) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega32u4;
    using usart = mcu::usart_01;
    using gpio_cfg = api::optimize<mcu::cfg::usart<usart, zoal::periph::usart_115200<16000000>>::apply>;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 4);
    EXPECT_EQ(c.values[0], cas_value(0xCC, 0xFF, 0x10, sizeof(uint8_t))); // UBRR1L
    EXPECT_EQ(c.values[1], cas_value(0xCD, 0xFF, 0x00, sizeof(uint8_t))); // UBRR1H
    EXPECT_EQ(c.values[2], cas_value(0xC8, 0xFF, 0x02, sizeof(uint8_t))); // UCSR1A
    EXPECT_EQ(c.values[3], cas_value(0xCA, 0xFF, 0x06, sizeof(uint8_t))); // UCSR1C
}

TEST(avr_atmega2560_usart, mux) {
    using namespace zoal::gpio;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using mcu = zoal::mcu::atmega2560;
    {
        using gpio_cfg = api::optimize<mcu::mux::usart<mcu::usart_00, mcu::pe_00, mcu::pe_01>::connect>;

        cas_collector c;
        type_list_iterator<gpio_cfg>::for_each(c);

        EXPECT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // No changes, just compile time checking
    }

    {
        using gpio_cfg = api::optimize<mcu::mux::usart<mcu::usart_01, mcu::pd_02, mcu::pd_03>::connect>;

        cas_collector c;
        type_list_iterator<gpio_cfg>::for_each(c);

        EXPECT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // No changes, just compile time checking
    }
    {
        using gpio_cfg = api::optimize<mcu::mux::usart<mcu::usart_02, mcu::ph_00, mcu::ph_01>::connect>;

        cas_collector c;
        type_list_iterator<gpio_cfg>::for_each(c);

        EXPECT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // No changes, just compile time checking
    }
    {
        using gpio_cfg = api::optimize<mcu::mux::usart<mcu::usart_03, mcu::pj_00, mcu::pj_01>::connect>;

        cas_collector c;
        type_list_iterator<gpio_cfg>::for_each(c);

        EXPECT_EQ(c.values.size(), 1);
        EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // No changes, just compile time checking
    }
}

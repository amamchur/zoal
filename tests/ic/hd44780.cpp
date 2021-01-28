#include "../test_utils/cas_collector.hpp"
#include "../test_utils/delay.hpp"
#include "../test_utils/ms_counter.hpp"

#include "gtest/gtest.h"
#include <zoal/board/arduino_uno.hpp>
#include <zoal/ct/type_list.hpp>
#include <zoal/ic/hd44780.hpp>

TEST(hd44780, uno_lcd_sheald) {
    using namespace zoal::ic;
    using namespace zoal::ct;
    using namespace zoal::tests;

    using counter = zoal::tests::ms_counter;
    using delay = zoal::tests::delay<counter>;
    using pcb = zoal::board::arduino_uno;

    using interface_type = zoal::ic::hd44780_interface_4bit<
        //
        delay,
        pcb::ard_d08, // typename mcu::pb_00
        pcb::ard_d09, // typename mcu::pb_01
        pcb::ard_d04, // typename mcu::pd_04
        pcb::ard_d05, // typename mcu::pd_05
        pcb::ard_d06, // typename mcu::pd_06
        pcb::ard_d07  // typename mcu::pd_07
        >;
    using address_selector = zoal::ic::hd44780_address_selector<16, 2>;
    using lcd_type = zoal::ic::hd44780<interface_type, address_selector>;
    using gpio_cfg = lcd_type::gpio_cfg;

    cas_collector c;
    type_list_iterator<gpio_cfg>::for_each(c);

    EXPECT_EQ(c.values.size(), 5);
    EXPECT_EQ(c.values[0], cas_value(0x00, 0x00, 0x00, sizeof(uint8_t))); // Dummy for GPIO port clock on
    EXPECT_EQ(c.values[1], cas_value(0x24, 0x00, 0x03, sizeof(uint8_t))); // DDRB |= 0x03
    EXPECT_EQ(c.values[2], cas_value(0x2A, 0x00, 0xF0, sizeof(uint8_t))); // DDRD |= 0xF0
    EXPECT_EQ(c.values[3], cas_value(0x25, 0x03, 0x00, sizeof(uint8_t))); // PORTB ~= 0x03
    EXPECT_EQ(c.values[4], cas_value(0x2B, 0xC0, 0x10, sizeof(uint8_t))); // PORTD = (PORTD & ~0xC0) | 0x10;
}

#include "gtest/gtest.h"
#include <zoal/io/analog_keypad.hpp>

TEST(analog_keypad, uno_lcd_sheald) {
    uint32_t events = 0xFF;
    uint32_t buttons = 0xFF;
    auto analog_keypad_handler = [&](zoal::io::button_event e, int button) {
        events = (events << 4) | (uint32_t)e;
        buttons = (buttons << 4) | (uint32_t)button;
    };

    using analog_keypad_type = zoal::io::analog_keypad<uint32_t, 5>;
    analog_keypad_type analog_keypad;
    analog_keypad.adc_values[0] = 0;
    analog_keypad.adc_values[1] = 131;
    analog_keypad.adc_values[2] = 308;
    analog_keypad.adc_values[3] = 481;
    analog_keypad.adc_values[4] = 722;

    analog_keypad.handle(analog_keypad_handler, 0, 1023);
    EXPECT_EQ(events, 0xFF);
    EXPECT_EQ(buttons, 0xFF);

    events = 0xFF;
    buttons = 0xFF;
    analog_keypad.handle(analog_keypad_handler, 1, 0);
    EXPECT_EQ(events, 0xFF01);
    EXPECT_EQ(buttons, 0xFF00);

    events = 0xFF;
    buttons = 0xFF;
    analog_keypad.handle(analog_keypad_handler, 50, 1023);
    EXPECT_EQ(events, 0xFF2);
    EXPECT_EQ(buttons, 0xFF0);

    events = 0xFF;
    buttons = 0xFF;
    analog_keypad.handle(analog_keypad_handler, 100, 400);
    EXPECT_EQ(events, 0xFF);
    EXPECT_EQ(buttons, 0xFF);

    events = 0xFF;
    buttons = 0xFF;
    analog_keypad.handle(analog_keypad_handler, 150, 400);
    EXPECT_EQ(events, 0xFF);
    EXPECT_EQ(buttons, 0xFF);

    events = 0xFF;
    buttons = 0xFF;
    analog_keypad.handle(analog_keypad_handler, 200, 1023);
    EXPECT_EQ(events, 0xFF);
    EXPECT_EQ(buttons, 0xFF);
}

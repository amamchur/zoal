#include "gtest/gtest.h"
#include <zoal/io/button_state_machine.hpp>

//104364 v:0 e:4
//104426 v:1 e:24
//104681 v:0 e:4
//104797 v:1 e:24

//106601 v:0 e:4
//106661 v:0 e:24
//106666 v:0 e:4
//106672 v:1 e:24

TEST(ButtonMachine, static_type_checking) {
    using namespace zoal::io;
    uint8_t button_state = 0;
    button_state_machine machine(5, 250);
    uint32_t dt = 1;
    auto state = machine.handle_button(dt, button_state, 1);
    auto events = state & button_state_trigger;
    EXPECT_EQ(state, 0x41);
    EXPECT_EQ(events, 0x0);
    button_state  = state & ~button_state_trigger;

    dt = 61;
    state = machine.handle_button(dt, button_state, 1);
    events = state & button_state_trigger;
    EXPECT_EQ(state, 0x01);
    EXPECT_EQ(events, 0x0);
    button_state  = state & ~button_state_trigger;

    dt = 66;
    state = machine.handle_button(dt, button_state, 1);
    events = state & button_state_trigger;
    EXPECT_EQ(state, 0x26);
    EXPECT_EQ(events, 0x04);
    button_state  = state & ~button_state_trigger;
}

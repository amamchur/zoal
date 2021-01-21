#include "gtest/gtest.h"
#include <zoal/io/button_state_machine.hpp>

static uintptr_t bsm_events = 0xFFFF;
static void callback(zoal::io::button_event e) {
    bsm_events = (bsm_events << 4) | (uintptr_t)e;
};

TEST(button_state_machine, base_flow) {
    zoal::io::button_state_machine bsm;
    // No changes idle
    bsm_events = 0xFFFF;
    bool switched = bsm.handle(0, 0, 5, 500, 30);
    EXPECT_EQ(switched, false);

    // Still no changes idle
    switched = bsm.handle(0, 10, 5, 500, 30);
    EXPECT_EQ(switched, false);

    // Button down -> debounce mode
    switched = bsm.handle(1, 20, 5, 500, 30);
    auto state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x73);

    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF01);

    // Still debounce mode
    switched = bsm.handle(1, 1, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    // Debounce finished
    switched = bsm.handle(1, 5, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    // One step more
    switched = bsm.handle(1, 6, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);

    // Button release time elapsed
    switched = bsm.handle(1, 505, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x55);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF1);

    switched = bsm.handle(1, 5, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x05);

    // Trigger key press after 30
    switched = bsm.handle(1, 35, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x55);

    switched = bsm.handle(1, 10, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x05);

    // Key up
    switched = bsm.handle(0, 11, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x92);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF2);

    switched = bsm.handle(0, 0, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x02);

    switched = bsm.handle(0, 50, 5, 500, 30);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x02);
}

TEST(button_state_machine, no_release_no_repeat) {
    zoal::io::button_state_machine bsm;

    // No changes idle
    bool switched = bsm.handle(0, 0, 5, 0, 0);
    EXPECT_EQ(switched, false);

    // Still no changes idle
    switched = bsm.handle(0, 10, 5, 0, 0);
    EXPECT_EQ(switched, false);

    // Button down -> debounce mode
    switched = bsm.handle(1, 20, 5, 0, 0);
    auto state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x73);

    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF01);

    // Still debounce mode
    switched = bsm.handle(1, 1, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    // Debounce finished
    switched = bsm.handle(1, 5, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    // One step more
    switched = bsm.handle(1, 6, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);

    // Button release time elapsed
    switched = bsm.handle(1, 505, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    switched = bsm.handle(1, 5, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);

    // Trigger key press after 30
    switched = bsm.handle(1, 35, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);

    switched = bsm.handle(1, 10, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);

    // Key up
    switched = bsm.handle(0, 11, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x92);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF2);

    switched = bsm.handle(0, 0, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x02);

    switched = bsm.handle(0, 50, 5, 0, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x02);
}

TEST(button_state_machine, no_release) {
    zoal::io::button_state_machine bsm;

    // No changes idle
    bool switched = bsm.handle(0, 0, 5, 500, 0);
    EXPECT_EQ(switched, false);

    // Still no changes idle
    switched = bsm.handle(0, 10, 5, 500, 0);
    EXPECT_EQ(switched, false);

    // Button down -> debounce mode
    bsm_events = 0xFFFF;
    switched = bsm.handle(1, 20, 5, 500, 0);
    auto state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x73);

    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF01);

    // Still debounce mode
    switched = bsm.handle(1, 1, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    // Debounce finished
    switched = bsm.handle(1, 5, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF);

    // One step more
    switched = bsm.handle(1, 6, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x03);

    // Button release time elapsed
    switched = bsm.handle(1, 505, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x55);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF1);

    switched = bsm.handle(1, 5, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x05);

    // Trigger key press after 30
    switched = bsm.handle(1, 35, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x05);

    switched = bsm.handle(1, 10, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x05);

    // Key up
    switched = bsm.handle(0, 11, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, true);
    EXPECT_EQ(state, 0x92);
    bsm_events = 0xFFFF;
    bsm.invoke_callback(callback);
    EXPECT_EQ(bsm_events, 0xFFFF2);

    switched = bsm.handle(0, 0, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x02);

    switched = bsm.handle(0, 50, 5, 500, 0);
    state = bsm.state();
    EXPECT_EQ(switched, false);
    EXPECT_EQ(state, 0x02);
}

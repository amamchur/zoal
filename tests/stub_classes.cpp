#include "gtest/gtest.h"

#include <zoal/ct/check.hpp>
#include <zoal/utils/helpers.hpp>
#include <zoal/gpio/pin.hpp>

using namespace zoal::gpio;
using namespace zoal::ct;

TEST(StubClasses, null_pin_should_be_callable) {  /* NOLINT */
    null_pin::low();
    null_pin::high();
    null_pin::toggle();
    null_pin::write(0);
    null_pin::mode<pin_mode::output>();
    {
        auto value = is_same<null_pin::port, null_port>::value;
        EXPECT_TRUE(value);
    }
    {
        auto value = null_pin::read();
        EXPECT_EQ(value, 0);
    }
    {
        auto value = null_pin::not_pin;
        EXPECT_TRUE(value);
    }
    {
        auto value = null_pin::offset;
        EXPECT_EQ(value, 0);
    }
    {
        auto value = null_pin::mask;
        EXPECT_EQ(value, 0);
    }
}

TEST(StubClasses, null_port_should_be_callable) {  /* NOLINT */
    null_port::enable();
    null_port::disable();

    null_port::low<1>();
    null_port::high<1>();
    null_port::toggle<1>();
    null_port::template mode<pin_mode::output, 1>();
    {
        auto value = null_port::read();
        EXPECT_EQ(value, 0);
    }
    {
        auto value = null_port::read();
        EXPECT_EQ(value, 0);
    }
    {
        auto value = null_port::address;
        EXPECT_EQ(value, 0);
    }
}

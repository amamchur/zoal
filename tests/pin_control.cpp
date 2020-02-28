#include "utils/accessor.hpp"

#include "gtest/gtest.h"
#include <zoal/gpio/pin.hpp>

using namespace zoal::gpio;

TEST(PinControl, pin_active_drain) {
    using namespace ::zoal::gpio;
    using ad = active_drain<null_pin>;
    using ah = active_high<null_pin>;
    using al = active_low<null_pin>;

    ad::on();
    ad::off();

    ah::on();
    ah::off();

    al::on();
    al::off();
}

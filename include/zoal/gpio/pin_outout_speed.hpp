#pragma once

#ifndef ZOAL_GPIO_PIN_OUTPUT_SPEED_HPP
#define ZOAL_GPIO_PIN_OUTPUT_SPEED_HPP

namespace zoal::gpio {
    enum class pin_output_speed {
        low,
        medium,
        high,
        very_high
    };
}

#endif

#pragma once

#ifndef ZOAL_GPIO_PIN_MODE_HPP
#define ZOAL_GPIO_PIN_MODE_HPP

#include <stdint.h>

namespace zoal { namespace gpio {
    enum class pin_mode {
        input_floating,
        input_pull_up,
        input_pull_down,
        output_open_drain,
        output_push_pull,

        input = static_cast<uint8_t>(input_floating),
        output = static_cast<uint8_t>(output_push_pull)
    };
}}

#endif

#ifndef ZOAL_GPIO_PIN_MODE_HPP
#define ZOAL_GPIO_PIN_MODE_HPP

namespace zoal { namespace gpio {
    enum class pin_mode {
        input_floating,
        input_pull_up,
        input_pull_down,
        output_open_drain,
        output_push_pull,

        input = input_floating,
        output = output_push_pull
    };
}}

#endif

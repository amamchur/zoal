#ifndef ZOAL_GPIO_FEATURE_HPP
#define ZOAL_GPIO_FEATURE_HPP

#include <stdint.h>

namespace zoal { namespace gpio {
    enum features {
        none = 0,
        output_speed = 1 << 0,
        output_open_drain = 1 << 1
    };

    template<class Port, features F>
    struct has_features {
        static constexpr bool value = (Port::feature & F) == F;
    };
}}

#endif

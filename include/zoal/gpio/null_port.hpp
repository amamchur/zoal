#ifndef ZOAL_GPIO_NULL_PORT_HPP
#define ZOAL_GPIO_NULL_PORT_HPP

#include "../utils/defs.hpp"
#include "pin_mode.hpp"

#include <stdint.h>

namespace zoal { namespace gpio {
    class null_port {
    public:
        using register_type = uint8_t;

        static constexpr uintptr_t address = 0;

        ZOAL_INLINE_IO static void power_on() {}

        ZOAL_INLINE_IO static void power_off() {}

        ZOAL_INLINE_IO static void enable() {}

        ZOAL_INLINE_IO static void disable() {}

        ZOAL_INLINE_IO static register_type read() {
            return 0;
        }

        template<register_type>
        ZOAL_INLINE_IO static void toggle() {}

        template<::zoal::gpio::pin_mode, register_type>
        class mode {};

        template<register_type>
        class low {};

        template<register_type>
        class high {};
    };
}}

#endif

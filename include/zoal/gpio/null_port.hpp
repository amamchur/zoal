#ifndef ZOAL_GPIO_NULL_PORT_HPP
#define ZOAL_GPIO_NULL_PORT_HPP

#include "../utils/defs.hpp"
#include "pin_mode.hpp"
#include "../mem/reg.hpp"

#include <stdint.h>

namespace zoal { namespace gpio {
    class null_port {
    public:
        using register_type = uint8_t;
        using null_reg = zoal::mem::reg<0, zoal::mem::reg_io::read_write, register_type, 0xFF>;

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
        ZOAL_INLINE_IO static void mode() {};

        template<register_type>
        ZOAL_INLINE_IO static void low() {};

        template<register_type>
        ZOAL_INLINE_IO static void high() {};

        template<register_type Mask>
        using low_cas = typename null_reg::template cas<0, 0>;

        template<register_type Mask>
        using high_cas = typename null_reg::template cas<0, 0>;
    };
}}

#endif

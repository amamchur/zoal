#ifndef ZOAL_GPIO_NULL_PORT_HPP
#define ZOAL_GPIO_NULL_PORT_HPP

#include "../mem/reg.hpp"
#include "../utils/defs.hpp"
#include "pin_mode.hpp"

#include <stdint.h>

namespace zoal { namespace gpio {
    using zoal::mem::callable_cas_list_variadic;
    using zoal::mem::null_cas_list;

    class null_port {
    public:
        using register_type = uint8_t;
        static constexpr zoal::gpio::features feature = zoal::gpio::features::none;
        static constexpr uintptr_t address = 0;

        ZOAL_INLINE_IO static register_type read() {
            return 0;
        }

        template<register_type>
        ZOAL_INLINE_IO static void toggle() {}

        template<::zoal::gpio::pin_mode, register_type>
        ZOAL_INLINE_IO static void mode(){};

        template<register_type>
        ZOAL_INLINE_IO static void low(){};

        template<register_type>
        ZOAL_INLINE_IO static void high(){};

        template<register_type Mask>
        using low_cas = null_cas_list;

        template<register_type Mask>
        using high_cas = null_cas_list;

        template<::zoal::gpio::pin_mode PinMode, register_type Mask>
        using mode_cas = null_cas_list;

        using clock_on_cas = null_cas_list;
        using clock_off_cas = null_cas_list;
        using clock_on = null_cas_list;
        using clock_off = null_cas_list;
    };
}}

#endif

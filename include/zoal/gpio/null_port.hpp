#ifndef ZOAL_GPIO_NULL_PORT_HPP
#define ZOAL_GPIO_NULL_PORT_HPP

#include "pin_mode.hpp"

namespace zoal { namespace gpio {
    class null_port {
    public:
        using register_type = uint8_t;

        null_port() = delete;

        static constexpr uintptr_t address = 0;

        static void power_on() {}

        static void power_off() {}

        static inline void enable() {}

        static inline void disable() {}

        static inline register_type read() {
            return 0;
        }

        template<register_type>
        static inline void low() {}

        template<register_type>
        static inline void high() {}

        template<register_type>
        static inline void toggle() {}

        template<::zoal::gpio::pin_mode, register_type>
        static inline void mode() {}
    };
}}

#endif

#ifndef ZOAL_GPIO_NULL_PORT_HPP
#define ZOAL_GPIO_NULL_PORT_HPP

#include "pin_mode.hpp"

namespace zoal { namespace gpio {
    class null_port {
    public:
        using register_type = uint8_t;

        null_port() = delete;

        static constexpr uintptr_t address = 0;
        
        static inline void enable() {}

        static inline void disable() {}

        static inline register_type read() {
            return 0;
        }

        static inline void low(register_type) {
        }

        static inline void high(register_type) {
        }

        static inline void toggle(register_type) {
        }

        template<::zoal::gpio::pin_mode>
        static inline void mode(register_type) {
        }

        template<::zoal::gpio::pin_mode, register_type>
        static inline void mode() {
        }
    };
}}

#endif

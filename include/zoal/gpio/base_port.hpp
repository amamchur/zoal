#ifndef ZOAL_GPIO_BASE_PORT_HPP
#define ZOAL_GPIO_BASE_PORT_HPP

#include <stdint.h>

namespace zoal { namespace gpio {
    template<class RegType, uintptr_t Address, RegType PinMask = 0xFF>
    class base_port {
    public:
        using register_type = uint8_t;
        static constexpr auto address = Address;
        static constexpr auto pin_mask = PinMask;
    };
}}

#endif

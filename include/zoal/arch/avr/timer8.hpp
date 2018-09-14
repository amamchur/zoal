#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER8BIT_HPP

#include "mcu_type.hpp"
#include "timer.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t N, bool Async = false, class ...Mixin>
    class timer8 : public timer<Address, N, Mixin...> {
    public:
        using self_type = timer8<Address, N, Async, Mixin...>;

        static constexpr bool async = Async;

        timer8() = delete;

        static typename self_type::word period() {
            return 0xFF;
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_FLAG_REGISTER_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_FLAG_REGISTER_HPP

#include <stdint.h>  /* NOLINT */
#include "../../../mem/accessor.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class timer_interrupt_flag_register {
    public:
        static constexpr uintptr_t TIFRx = 0;

        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        template<uint8_t Timer>
        static inline void clear_counter_flag() {
            *accessor<TIFRx>::p = 0;
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void clear_channel_flag() {
            *accessor<TIFRx>::p = 0;
        }
    };
}}}

#endif

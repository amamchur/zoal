#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_FLAG_REGISTER_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_FLAG_REGISTER_HPP

#include <stdint.h>  /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class timer_interrupt_flag_register {
    public:
        static constexpr uintptr_t TIFRx = 0;

        using TIFRx = zoal::mem::reg<Address, zoal::mem::reg_io::read_write, uint8_t, 0x27>;

        template<uint8_t Timer>
        static inline void clear_counter_flag() {
            TIFRx::ref() = 0;
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void clear_channel_flag() {
            TIFRx::ref()= 0;
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_FLAG_REGISTER_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_FLAG_REGISTER_HPP

#include <stdint.h>  /* NOLINT */
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t BaseAddress>
    class timer_interrupt_flag_register {
    public:
        typedef timer_interrupt_flag_register<BaseAddress> Class;

        static constexpr uintptr_t TIFRx = 0;

        template<uint8_t Timer>
        static inline void clear_counter_flag() {
            mem[TIFRx] = 0;
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void clear_channel_flag() {
            mem[TIFRx] = 0;
        }

    private:
        static zoal::utils::memory_segment<uint8_t, BaseAddress> mem;
    };

    template<uintptr_t BaseAddress>
    zoal::utils::memory_segment<uint8_t, BaseAddress> timer_interrupt_flag_register<BaseAddress>::mem;
}}}

#endif

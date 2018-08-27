#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP

#include <stdint.h>  /* NOLINT */
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t Count>
    class timer_interrupt_flags_vector {
    private:
        enum {
            TOVx = 0,
            OCFAx = 1,
            OCFBx = 2
        };
    public:
        template<uint8_t Timer>
        static inline void clear_counter_flag() {
            static_assert(Timer < Count, "Timer index is out of range");
            mem[Timer] = static_cast<uint8_t>(~(1 << TOVx));
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void clear_channel_flag() {
            static_assert(Timer < Count, "Timer index is out of range");
            switch (Channel) {
                case 0:
                    mem[Timer] = static_cast<uint8_t>(~(1 << OCFAx));
                    break;
                case 1:
                    mem[Timer] = static_cast<uint8_t>(~(1 << OCFBx));
                    break;
                default:
                    break;
            }
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t Count>
    zoal::utils::memory_segment<uint8_t, Address> timer_interrupt_flags_vector<Address, Count>::mem;
}}}

#endif

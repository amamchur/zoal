#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP

#include "../../../utils/memory_segment.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t Count>
    class timer_interrupt_mask_vector {
    private:
        enum { TOIEx = 0, OCIEAx = 1, OCIEBx = 2 };

    public:
        template<uint8_t Timer>
        static inline void enable_overflow_interrupt() {
            static_assert(Timer < Count, "Timer index is out of range");

            mem[Timer] |= (1 << TOIEx);
        }

        template<uint8_t Timer>
        static inline void disable_overflow_interrupt() {
            static_assert(Timer < Count, "Timer index is out of range");

            mem[Timer] &= ~(1 << TOIEx);
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void enable_compare_match_interrupt() {
            static_assert(Timer < Count, "Timer index is out of range");
            static_assert(Channel < 2, "Channel index is out of range");

            switch (Channel) {
            case 0:
                mem[Timer] |= (1 << OCIEAx);
                break;
            case 1:
                mem[Timer] |= (1 << OCIEBx);
                break;
            default:
                break;
            }
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void disable_compare_match_interrupt() {
            static_assert(Timer < Count, "Timer index is out of range");
            static_assert(Channel < 2, "Channel index is out of range");

            switch (Channel) {
            case 0:
                mem[Timer] &= ~(1 << OCIEAx);
                break;
            case 1:
                mem[Timer] &= ~(1 << OCIEBx);
                break;
            default:
                break;
            }
        }

    private:
        static zoal::utils::memory_segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t Count>
    zoal::utils::memory_segment<uint8_t, Address> timer_interrupt_mask_vector<Address, Count>::mem;
}}}

#endif

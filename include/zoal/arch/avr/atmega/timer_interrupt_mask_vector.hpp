#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP

#include "../../../mem/accessor.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class timer_interrupt_mask_vector {
    private:
        enum { TOIEx = 0, OCIEAx = 1, OCIEBx = 2 };

    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        template<uint8_t Timer>
        static void enable_overflow_interrupt() {
            *accessor<Timer>::p |= (1 << TOIEx);
        }

        template<uint8_t Timer>
        static void disable_overflow_interrupt() {
            *accessor<Timer>::p &= ~(1 << TOIEx);
        }

        template<uint8_t Timer, uint8_t Channel>
        static void enable_compare_match_interrupt() {
            switch (Channel) {
            case 0:
                *accessor<Timer>::p |= (1 << OCIEAx);
                break;
            case 1:
                *accessor<Timer>::p |= (1 << OCIEBx);
                break;
            default:
                break;
            }
        }

        template<uint8_t Timer, uint8_t Channel>
        static void disable_compare_match_interrupt() {
            switch (Channel) {
            case 0:
                *accessor<Timer>::p &= ~(1 << OCIEAx);
                break;
            case 1:
                *accessor<Timer>::p &= ~(1 << OCIEBx);
                break;
            default:
                break;
            }
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_VECTOR_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class timer_interrupt_mask_vector {
    private:
        enum { TOIEx = 0, OCIExA = 1, OCIExB = 2 };

    public:
        template<uint32_t No>
        using TIMRx = zoal::mem::reg<Address + No, zoal::mem::reg_io::read_write, uint8_t, 0x27>;

        template<uint8_t Timer>
        static void enable_overflow_interrupt() {
            TIMRx<Timer>::ref() |= (1 << TOIEx);
        }

        template<uint8_t Timer>
        static void disable_overflow_interrupt() {
            TIMRx<Timer>::ref() &= ~(1 << TOIEx);
        }

        template<uint8_t Timer, uint8_t Channel>
        static void enable_compare_match_interrupt() {
            switch (Channel) {
            case 0:
                TIMRx<Timer>::ref() |= (1 << OCIExA);
                break;
            case 1:
                TIMRx<Timer>::ref() |= (1 << OCIExB);
                break;
            default:
                break;
            }
        }

        template<uint8_t Timer, uint8_t Channel>
        static void disable_compare_match_interrupt() {
            switch (Channel) {
            case 0:
                TIMRx<Timer>::ref() &= ~(1 << OCIExA);
                break;
            case 1:
                TIMRx<Timer>::ref() &= ~(1 << OCIExB);
                break;
            default:
                break;
            }
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_REGISTER_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_REGISTER_HPP

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t toieMask, uint8_t ocieaMask, uint8_t ociebMask>
    class timer_interrupt_mask_register {
    public:
        using TIMSKs = zoal::mem::reg<Address, zoal::mem::reg_io::read_write, uint8_t, 0x27>;

        template<uint8_t Timer>
        static inline void enable_overflow_interrupt() {
            TIMSKs::ref()|= toieMask;
        }

        template<uint8_t Timer>
        static inline void disable_overflow_interrupt() {
            TIMSKs::ref() &= ~toieMask;
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void enable_compare_match_interrupt() {
            static_assert(Channel < 2, "Channel index is out of range");
            switch (Channel) {
            case 0:
                TIMSKs::ref() |= ocieaMask;
                break;
            case 1:
                TIMSKs::ref() |= ociebMask;
                break;
            default:
                break;
            }
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void disable_compare_match_interrupt() {
            static_assert(Channel < 2, "Channel index is out of range");

            switch (Channel) {
            case 0:
                TIMSKs::ref() &= ~ocieaMask;
                break;
            case 1:
                TIMSKs::ref() &= ~ociebMask;
                break;
            default:
                break;
            }
        }
    };
}}}

#endif

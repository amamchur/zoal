#ifndef ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_REGISTER_HPP
#define ZOAL_ARCH_ATMEL_AVR_TIMER_INTERRUPT_MASK_REGISTER_HPP

#include <stdint.h>  /* NOLINT */
#include "..mem/segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address, uint8_t toieMask, uint8_t ocieaMask, uint8_t ociebMask>
    class timer_interrupt_mask_register {
    public:

        static constexpr uintptr_t TIMSKs = 0;

        template<uint8_t Timer>
        static inline void enable_overflow_interrupt() {
            mem[TIMSKs] |= toieMask;
        }

        template<uint8_t Timer>
        static inline void disable_overflow_interrupt() {
            mem[TIMSKs] &= ~toieMask;
        }

        template<uint8_t Timer, uint8_t Channel>
        static inline void enable_compare_match_interrupt() {
            static_assert(Channel < 2, "Channel index is out of range");
            switch (Channel) {
                case 0:
                    mem[TIMSKs] |= ocieaMask;
                    break;
                case 1:
                    mem[TIMSKs] |= ociebMask;
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
                    mem[TIMSKs] &= ~ocieaMask;
                    break;
                case 1:
                    mem[TIMSKs] &= ~ociebMask;
                    break;
                default:
                    break;
            }
        }

    private:
        static zoal::mem::segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t toieMask, uint8_t ocieaMask, uint8_t ociebMask>
    zoal::mem::segment<uint8_t, Address> timer_interrupt_mask_register<Address, toieMask, ocieaMask, ociebMask>::mem;
}}}

#endif

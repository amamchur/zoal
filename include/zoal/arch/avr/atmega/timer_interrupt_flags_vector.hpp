#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP

#include "../../../mem/segment.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class timer_interrupt_flags_vector {
    private:
        enum { TOVx = 0, OCFAx = 1, OCFBx = 2 };

    public:
        template<uint8_t Timer>
        static void clear_counter_flag() {
            mem[Timer] = static_cast<uint8_t>(~(1 << TOVx));
        }

        template<uint8_t Timer, uint8_t Channel>
        static void clear_channel_flag() {
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
        static zoal::mem::segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address>
    zoal::mem::segment<uint8_t, Address> timer_interrupt_flags_vector<Address>::mem;
}}}

#endif

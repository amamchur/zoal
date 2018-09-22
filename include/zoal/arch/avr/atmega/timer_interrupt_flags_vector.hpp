#ifndef ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP
#define ZOAL_ARCH_AVR_ATMEGA_TIMER_INTERRUPT_FLAG_VECTOR_HPP

#include "../../../mem/accessor.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr {
    template<uintptr_t Address>
    class timer_interrupt_flags_vector {
    private:
        enum { TOVx = 0, OCFAx = 1, OCFBx = 2 };

    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint8_t, Address, Offset>;

        template<uint8_t Timer>
        static void clear_counter_flag() {
            *accessor<Timer>::p = static_cast<uint8_t>(~(1 << TOVx));
        }

        template<uint8_t Timer, uint8_t Channel>
        static void clear_channel_flag() {
            switch (Channel) {
            case 0:
                *accessor<Timer>::p = static_cast<uint8_t>(~(1 << OCFAx));
                break;
            case 1:
                *accessor<Timer>::p = static_cast<uint8_t>(~(1 << OCFBx));
                break;
            default:
                break;
            }
        }
    };
}}}

#endif

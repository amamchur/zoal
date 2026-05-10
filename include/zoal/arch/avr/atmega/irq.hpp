#ifndef ZOAL_ARCH_AVR_ATMEGA_IRQ_HPP
#define ZOAL_ARCH_AVR_ATMEGA_IRQ_HPP

#include "../../../utils/interrupts.hpp"
#include "timer_interrupt_flags_vector.hpp"
#include "timer_interrupt_mask_vector.hpp"

namespace zoal::arch::avr::atmega {
    class irq : public utils::interrupts {
        using tifrs = timer_interrupt_flags_vector<0x35>;
        using timrs = timer_interrupt_mask_vector<0x6E>;

    public:
        template<class T>
        class timer {
        public:
            using enable_overflow_interrupt = timrs::enable_overflow_interrupt<T::no>;
            using disable_overflow_interrupt = timrs::disable_overflow_interrupt<T::no>;
            using clear_overflow_interrupt_flag = timrs::enable_overflow_interrupt<T::no>;

            template<uint8_t Channel>
            using enable_compare_match_interrupt = timrs::enable_compare_match_interrupt<T::no, Channel>;

            template<uint8_t Channel>
            using disable_compare_match_interrupt = timrs::disable_compare_match_interrupt<T::no, Channel>;

            template<uint8_t Channel>
            using clear_channel_interrupt_flag = tifrs::clear_channel_flag<T::no, Channel>;
        };
    };
}

#endif

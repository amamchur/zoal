#ifndef ZOAL_ARCH_AVR_ATMEGA_IRQ_HPP
#define ZOAL_ARCH_AVR_ATMEGA_IRQ_HPP

#include "../../../utils/interrupts.hpp"
#include "timer_interrupt_flags_vector.hpp"
#include "timer_interrupt_mask_vector.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class irq : public zoal::utils::interrupts {
    private:
        using tifrs = avr::timer_interrupt_flags_vector<0x35>;
        using timrs = avr::timer_interrupt_mask_vector<0x6E>;

    public:
        template<class T>
        class timer {
        public:
            using enable_overflow_interrupt = timrs::template enable_overflow_interrupt<T::no>;
            using disable_overflow_interrupt = timrs::template disable_overflow_interrupt<T::no>;
            using clear_overflow_interrupt_flag = timrs::template enable_overflow_interrupt<T::no>;

            template<uint8_t Channel>
            using enable_compare_match_interrupt = timrs::template enable_compare_match_interrupt<T::no, Channel>;

            template<uint8_t Channel>
            using disable_compare_match_interrupt = timrs::template disable_compare_match_interrupt<T::no, Channel>;

            template<uint8_t Channel>
            using clear_channel_interrupt_flag = tifrs::template clear_channel_flag<T::no, Channel>;
        };
    };
}}}}

#endif

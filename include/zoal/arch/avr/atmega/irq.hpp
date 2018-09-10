#ifndef ZOAL_ARCH_AVR_ATMEGA_IRQ_HPP
#define ZOAL_ARCH_AVR_ATMEGA_IRQ_HPP

#include "timer_interrupt_flags_vector.hpp"
#include "timer_interrupt_mask_vector.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class irq {
    private:
        using tifrs = avr::timer_interrupt_flags_vector<0x35>;
        using timrs = avr::timer_interrupt_mask_vector<0x6E>;
    public:
        template<class T>
        class timer {
        public:
            static void enable_overflow_interrupt() {
                timrs::template enable_overflow_interrupt<T::no>();
            }

            static void disable_overflow_interrupt() {
                timrs::template disable_overflow_interrupt<T::no>();
            }

            static void clear_overflow_interrupt_flag() {
                tifrs::template clear_counter_flag<T::no>();
            }

            template<uint8_t Channel>
            static void enable_compare_match_interrupt() {
//                static_assert(channel < channels_count, "Channel index is out of range");
                timrs::template enable_compare_match_interrupt<T::no, Channel>();
            }

            template<uint8_t Channel>
            static void disable_compare_match_interrupt() {
//                static_assert(channel < channels_count, "Channel index is out of range");
                timrs::template disable_compare_match_interrupt<T::no, Channel>();
            }

            template<uint8_t Channel>
            static void clear_channel_interrupt_flag() {
//                static_assert(channel < channels_count, "Channel index is out of range");
                tifrs::template clear_channel_flag<T::no, Channel>();
            }
        };
    };
}}}}

#endif

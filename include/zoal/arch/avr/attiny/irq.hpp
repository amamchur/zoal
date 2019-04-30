#ifndef ZOAL_ARCH_AVR_ATTINY_IRQ_HPP
#define ZOAL_ARCH_AVR_ATTINY_IRQ_HPP

#include "../../../mem/accessor.hpp"
#include "../utils/interrupts.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace avr { namespace attiny {
class irq : public zoal::utils::interrupts {
    public:
        template<class T>
        class timer {
        public:
            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, T::address, Offset>;

            static void enable_overflow_interrupt() {
                *accessor<T::TIMSKx>::p |= T::TIMSKx_TOIEx;
            }

            static void disable_overflow_interrupt() {
                *accessor<T::TIMSKx>::p &= ~T::TIMSKx_TOIEx;
            }

            static void clear_overflow_interrupt_flag() {
                *accessor<T::TIFRx>::p &= ~T::TIFRx_TOVx;
            }

            template<uint8_t Channel>
            static void enable_compare_match_interrupt() {
                *accessor<T::TIMSKx>::p |= (Channel == 0 ? T::TIMSKx_OCIExA : T::TIMSKx_OCIExB);
            }

            template<uint8_t Channel>
            static void disable_compare_match_interrupt() {
                *accessor<T::TIMSKx>::p &= ~(Channel == 0 ? T::TIMSKx_OCIExA : T::TIMSKx_OCIExB);
            }

            template<uint8_t Channel>
            static void clear_channel_interrupt_flag() {
                *accessor<T::TIFRx>::p &= ~(Channel == 0 ? T::TIFRx_OCFxA : T::TIFRx_OCFxB);
            }
        };

        template<class A>
        class adc {
        public:
            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, A::address, Offset>;

            static void enable() {
                *accessor<A::ADCSRAx>::p |= 0x08;
            }

            static void disable() {
                *accessor<A::ADCSRAx>::p &= ~0x08;
            }
        };
    };
}}}}

#endif

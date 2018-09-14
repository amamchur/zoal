#ifndef ZOAL_ARCH_AVR_ATTINY_IRQ_HPP
#define ZOAL_ARCH_AVR_ATTINY_IRQ_HPP

#include <stdint.h>

namespace zoal { namespace arch { namespace avr { namespace attiny {
    class irq {
    public:
        template<class T>
        class timer {
        public:
            static void enable_overflow_interrupt() {
                memory_segment<uint8_t, T::address> mem;
                mem[T::TIMSKx] |= T::TIMSKx_TOIEx;
                mem.happyInspection();
            }

            static void disable_overflow_interrupt() {
                memory_segment<uint8_t, T::address> mem;
                mem[T::TIMSKx] &= ~T::TIMSKx_TOIEx;
                mem.happyInspection();
            }

            static void clear_overflow_interrupt_flag() {
                memory_segment<uint8_t, T::address> mem;
                mem[T::TIFRx] &= ~T::TIFRx_TOVx;
                mem.happyInspection();
            }

            template<uint8_t Channel>
            static void enable_compare_match_interrupt() {
                memory_segment<uint8_t, T::address> mem;
                mem[T::TIMSKx] |= (Channel == 0 ? T::TIMSKx_OCIExA : T::TIMSKx_OCIExB);
                mem.happyInspection();
            }

            template<uint8_t Channel>
            static void disable_compare_match_interrupt() {
                memory_segment<uint8_t, T::address> mem;
                mem[T::TIMSKx] &= ~(Channel == 0 ? T::TIMSKx_OCIExA : T::TIMSKx_OCIExB);
                mem.happyInspection();
            }

            template<uint8_t Channel>
            static void clear_channel_interrupt_flag() {
                memory_segment<uint8_t, T::address> mem;
                mem[T::TIFRx] &= ~(Channel == 0 ? T::TIFRx_OCFxA : T::TIFRx_OCFxB);
                mem.happyInspection();
            }
        };

        template<class A>
        class adc {
        public:
            static void enable() {
                memory_segment<uint8_t, A::address> mem;
                mem[A::ADCSRAx] |= 0x08;
                mem.happyInspection();
            }

            static void disable() {
                memory_segment<uint8_t, A::address> mem;
                mem[A::ADCSRAx] |= 0x08;
                mem.happyInspection();
            }
        };
    };
}}}}

#endif

#ifndef ZOAL_ARCH_STM32F3_GENERAL_PURPOSE_TIMER_HPP
#define ZOAL_ARCH_STM32F3_GENERAL_PURPOSE_TIMER_HPP

#include <stdint.h>
#include "../../../periph/timer_mode.hpp"
#include "../../../periph/timer_interrupt.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace stm32f3 {
    static constexpr uintptr_t TIMx_CR1 = 0x00;
    static constexpr uintptr_t TIMx_CR2 = 0x04;
    static constexpr uintptr_t TIMx_SMCR = 0x08;
    static constexpr uintptr_t TIMx_DIER = 0x0C;
    static constexpr uintptr_t TIMx_SR = 0x10;
    static constexpr uintptr_t TIMx_EGR = 0x14;
    static constexpr uintptr_t TIMx_CCMR1 = 0x18;
    static constexpr uintptr_t TIMx_CCMR2 = 0x1C;
    static constexpr uintptr_t TIMx_CCER = 0x20;
    static constexpr uintptr_t TIMx_CNT = 0x24;
    static constexpr uintptr_t TIMx_PSC = 0x28;
    static constexpr uintptr_t TIMx_ARR = 0x2C;
    static constexpr uintptr_t TIMx_CCR1 = 0x34;
    static constexpr uintptr_t TIMx_CCR2 = 0x38;
    static constexpr uintptr_t TIMx_CCR3 = 0x3C;
    static constexpr uintptr_t TIMx_CCR4 = 0x40;
    static constexpr uintptr_t TIMx_DCR = 0x48;
    static constexpr uintptr_t TIMx_DMAR = 0x4C;

    template<uintptr_t Address, uint8_t N, class ... Mixin>
    class general_purpose_timer : public Mixin ... {
    public:
        using self_type = general_purpose_timer<Address, N, Mixin...>;
        using word = uint16_t;

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t resolution = sizeof(word) * 8;

        static inline void enable() {
            mem[TIMx_CR1] |= 1; // TIM_CR1_CEN
        }

        static inline void disable() {
            mem[TIMx_CR1] &= static_cast<uint32_t>(1);
        }

        static inline void counter(word value) {
            mem[TIMx_CNT] = value;
        }

        static inline word counter() {
            return mem[TIMx_CNT];
        }

        template<zoal::periph::timer_interrupt TimerInterrupt>
        static inline void enable_interrupt() {
        }

        static inline void enable_overflow_interrupt() {
            mem[TIMx_DIER] |= 1;
        }

        static inline void disable_overflow_interrupt() {
            mem[TIMx_DIER] &= static_cast<uint32_t>(1);
        }

        template<uint32_t Division>
        static inline void clock_division() {
            switch (Division) {
            case 1:
                mem[TIMx_CR1] = (mem[TIMx_CR1] & ~static_cast<uint32_t >(0x300));
                break;
            case 2:
                mem[TIMx_CR1] = (mem[TIMx_CR1] & ~static_cast<uint32_t >(0x300)) | 0x100;
                break;
            case 4:
                mem[TIMx_CR1] = (mem[TIMx_CR1] & ~static_cast<uint32_t >(0x300)) | 0x200;
                break;
            default:
                break;
            }
        }

        template<uint32_t Period>
        static inline void period() {
            mem[TIMx_ARR] = Period;
        }

        template<uint32_t Prescaler>
        static inline void prescaler() {
            mem[TIMx_PSC] = Prescaler;
        }

        template<zoal::periph::timer_mode TimerMode>
        static inline void mode() {
//            switch (TimerMode) {
//            case zoal::periph::timer_mode::fast_pwm_8bit:
//            case zoal::periph::timer_mode::phase_correct_8bit:
//                mem[TIMx_ARR] = 0xFF;
//                break;
//            case zoal::periph::timer_mode::fast_pwm_9bit:
//            case zoal::periph::timer_mode::phase_correct_9bit:
//                mem[TIMx_ARR] = 0x1FF;
//                break;
//            case zoal::periph::timer_mode::fast_pwm_10bit:
//            case zoal::periph::timer_mode::phase_correct_10bit:
//                mem[TIMx_ARR] = 0x3FF;
//                break;
//            case zoal::periph::timer_mode::fast_pwm_16bit:
//            case zoal::periph::timer_mode::phase_correct_16bit:
//                mem[TIMx_ARR] = 0xFFFF;
//                break;
//            default:
//                break;
//            }
        }

        template<class ClockSource>
        static inline void select_clock_source() {
            mem[TIMx_PSC] = ClockSource::asas;
        }

    private:
        template <class T, zoal::periph::timer_interrupt TimerInterrupt, int Channel>
        class timer_interrupt_control;

        static zoal::utils::memory_segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, class ... Mixins>
    zoal::utils::memory_segment<uint32_t, Address> general_purpose_timer<Address, N, Mixins...>::mem;
}}}

#endif

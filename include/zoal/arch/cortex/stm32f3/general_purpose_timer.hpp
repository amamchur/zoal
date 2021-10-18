#ifndef ZOAL_ARCH_STM32F3_GENERAL_PURPOSE_TIMER_HPP
#define ZOAL_ARCH_STM32F3_GENERAL_PURPOSE_TIMER_HPP

#include "../../../periph/timer_interrupt.hpp"
#include "../../../periph/timer_mode.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f3 {
    template<uintptr_t Address, uint8_t N, class... Mixin>
    class general_purpose_timer : public Mixin... {
    public:
        using self_type = general_purpose_timer<Address, N, Mixin...>;
        using word = uint16_t;

        using TIMx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_SMCR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_DIER = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_SR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_EGR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCMR1 = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCMR2 = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCER = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CNT = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_PSC = zoal::mem::reg<Address + 0x28, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_ARR = zoal::mem::reg<Address + 0x2C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCR1 = zoal::mem::reg<Address + 0x34, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCR2 = zoal::mem::reg<Address + 0x38, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCR3 = zoal::mem::reg<Address + 0x3C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_CCR4 = zoal::mem::reg<Address + 0x40, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_DCR = zoal::mem::reg<Address + 0x48, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMx_DMAR = zoal::mem::reg<Address + 0x4C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static constexpr uintptr_t address = Address;
        static constexpr uint8_t no = N;
        static constexpr uint8_t resolution = sizeof(word) * 8;

        static inline void enable() {
            TIMx_CR1::ref() |= 1; // TIM_CR1_CEN
        }

        static inline void disable() {
            TIMx_CR1::ref() &= static_cast<uint32_t>(1);
        }

        static inline void counter(word value) {
            TIMx_CNT::ref() = value;
        }

        static inline uint32_t counter() {
            return TIMx_CNT::ref();
        }

        template<zoal::periph::timer_interrupt TimerInterrupt>
        static inline void enable_interrupt() {}

        static inline void enable_overflow_interrupt() {
            TIMx_DIER::ref() |= 1;
        }

        static inline void disable_overflow_interrupt() {
            TIMx_DIER::ref() &= static_cast<uint32_t>(1);
        }

        template<uint32_t Division>
        static inline void clock_division() {
            switch (Division) {
            case 1:
                TIMx_CR1::ref() = (TIMx_CR1::ref() & ~static_cast<uint32_t>(0x300));
                break;
            case 2:
                TIMx_CR1::ref() = (TIMx_CR1::ref() & ~static_cast<uint32_t>(0x300)) | 0x100;
                break;
            case 4:
                TIMx_CR1::ref() = (TIMx_CR1::ref() & ~static_cast<uint32_t>(0x300)) | 0x200;
                break;
            default:
                break;
            }
        }

        template<uint32_t Period>
        static inline void period() {
            TIMx_ARR::ref() = Period;
        }

        template<uint32_t Prescaler>
        static inline void prescaler() {
            TIMx_PSC::ref() = Prescaler;
        }

        template<zoal::periph::timer_mode TimerMode>
        static inline void mode() {
            //            switch (TimerMode) {
            //            case zoal::periph::timer_mode::fast_pwm_8bit:
            //            case zoal::periph::timer_mode::phase_correct_8bit:
            //                TIMx_ARR::ref() = 0xFF;
            //                break;
            //            case zoal::periph::timer_mode::fast_pwm_9bit:
            //            case zoal::periph::timer_mode::phase_correct_9bit:
            //                TIMx_ARR::ref() = 0x1FF;
            //                break;
            //            case zoal::periph::timer_mode::fast_pwm_10bit:
            //            case zoal::periph::timer_mode::phase_correct_10bit:
            //                TIMx_ARR::ref() = 0x3FF;
            //                break;
            //            case zoal::periph::timer_mode::fast_pwm_16bit:
            //            case zoal::periph::timer_mode::phase_correct_16bit:
            //                TIMx_ARR::ref() = 0xFFFF;
            //                break;
            //            default:
            //                break;
            //            }
        }

        template<class ClockSource>
        static inline void select_clock_source() {
            TIMx_PSC::ref() = ClockSource::asas;
        }

    private:
        template<class T, zoal::periph::timer_interrupt TimerInterrupt, int Channel>
        class timer_interrupt_control;
    };
}}}

#endif

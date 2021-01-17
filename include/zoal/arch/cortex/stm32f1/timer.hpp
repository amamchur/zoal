#ifndef ZOAL_ARCH_STM32F1_TIMER_HPP
#define ZOAL_ARCH_STM32F1_TIMER_HPP

#include "../../../periph/timer.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class... Mixin>
    class timer : public Mixin... {
    public:
        using TIMERx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_SMCR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_DIER = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_SR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_EGR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCMR1 = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCMR2 = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCER = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CNT = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_PSC = zoal::mem::reg<Address + 0x28, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_ARR = zoal::mem::reg<Address + 0x2C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_RCR = zoal::mem::reg<Address + 0x30, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCR1 = zoal::mem::reg<Address + 0x34, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCR2 = zoal::mem::reg<Address + 0x38, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCR3 = zoal::mem::reg<Address + 0x3C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_CCR4 = zoal::mem::reg<Address + 0x40, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_BDTR = zoal::mem::reg<Address + 0x44, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_DCR = zoal::mem::reg<Address + 0x48, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_DMAR = zoal::mem::reg<Address + 0x4C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using TIMERx_OR = zoal::mem::reg<Address + 0x50, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static constexpr uint32_t TIMERx_CR1_CKD = 1 << 8 | 1 << 9;
        static constexpr uint32_t TIMERx_CR1_ARPE = 1 << 7;
        static constexpr uint32_t TIMERx_CR1_CMS = 1 << 5 | 1 << 6;
        static constexpr uint32_t TIMERx_CR1_DIR = 1 << 4;
        static constexpr uint32_t TIMERx_CR1_OPM = 1 << 3;
        static constexpr uint32_t TIMERx_CR1_URS = 1 << 2;
        static constexpr uint32_t TIMERx_CR1_UDIS = 1 << 1;
        static constexpr uint32_t TIMERx_CR1_CEN = 1 << 0;
        static constexpr uint32_t TIMERx_EGR_TG = 1 << 6;
        static constexpr uint32_t TIMERx_EGR_UG = 1 << 0;
        static constexpr uint32_t TIMERx_DIER_UIE = 1 << 0;
        static constexpr uint32_t TIMERx_SR_UIF = 1 << 0;

        using enable_cas = zoal::ct::type_list<typename TIMERx_CR1::template cas<0, TIMERx_CR1_CEN>>;
        using disable_cas = zoal::ct::type_list<typename TIMERx_CR1::template cas<TIMERx_CR1_CEN, 0>>;

        static inline void enable() {
            TIMERx_CR1::ref() |= TIMERx_CR1_CEN;
        }

        static inline void disable() {
            TIMERx_CR1::ref() &= ~TIMERx_CR1_CEN;
        }

        static inline void enable_update_irq() {
            TIMERx_DIER::ref() |= TIMERx_DIER_UIE;
        }

        static inline void disable_update_irq() {
            TIMERx_DIER::ref() &= ~TIMERx_DIER_UIE;
        }

        template<class UpdateCallback>
        static inline void update_handler(UpdateCallback ucb) {
            if ((TIMERx_SR::ref() & TIMERx_SR_UIF) == TIMERx_SR_UIF) {
                TIMERx_SR::ref() &= ~TIMERx_SR_UIF;
                ucb();
            }
        }
    };
}}}

#endif

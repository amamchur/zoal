#ifndef ZOAL_ARCH_STM32F3_A2DC_COMMON_REGS_HPP
#define ZOAL_ARCH_STM32F3_A2DC_COMMON_REGS_HPP

#include "../../../mem/accessor.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f3 {

    template<uintptr_t Address>
    class adc_common_regs {
    public:
        template<uintptr_t Offset>
        using accessor = zoal::mem::accessor<uint32_t , Address, Offset>;

        static constexpr uintptr_t ADCx_CSR = 0x00;
        static constexpr uintptr_t ADCx_CCR = 0x04;
        static constexpr uintptr_t ADCx_CDR = 0x0C;

        adc_common_regs() = delete;

        static inline void reset() {
            *accessor<ADCx_CSR>::p = 0;
            *accessor<ADCx_CCR>::p = 0;
            *accessor<ADCx_CDR>::p = 0;
        }
    };
}}}

#endif

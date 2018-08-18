#ifndef ZOAL_ARCH_STM32F3_A2DC_COMMON_REGS_HPP
#define ZOAL_ARCH_STM32F3_A2DC_COMMON_REGS_HPP

#include <stdint.h>
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace stm32f3 {

    template<uintptr_t Address>
    class adc_common_regs {
    public:
        static constexpr uintptr_t ADCx_CSR = 0x00;
        static constexpr uintptr_t ADCx_CCR = 0x04;
        static constexpr uintptr_t ADCx_CDR = 0x0C;

        adc_common_regs() = delete;

        static inline void reset() {
            mem[ADCx_CSR] = 0;
            mem[ADCx_CCR] = 0;
            mem[ADCx_CDR] = 0;
        }

    private:
        static zoal::utils::memory_segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address>
    zoal::utils::memory_segment<uint32_t, Address> adc_common_regs<Address>::mem;
}}}

#endif

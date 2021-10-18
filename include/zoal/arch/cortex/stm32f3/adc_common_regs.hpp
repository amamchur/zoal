#ifndef ZOAL_ARCH_STM32F3_A2DC_COMMON_REGS_HPP
#define ZOAL_ARCH_STM32F3_A2DC_COMMON_REGS_HPP

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f3 {

    template<uintptr_t Address>
    class adc_common_regs {
    public:
        using ADCx_CSR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t , 0xFFFFFFFF>;
        using ADCx_CCR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADCx_CDR = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;

        static inline void reset() {
            ADCx_CSR::ref() = 0;
            ADCx_CCR::ref() = 0;
            ADCx_CDR::ref() = 0;
        }
    };
}}}

#endif

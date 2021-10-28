#ifndef ZOAL_ADC_COMMON_STM32F3_HPP
#define ZOAL_ADC_COMMON_STM32F3_HPP

#include "../../../mem/reg.hpp"

namespace zoal { namespace arch { namespace stm32f3 {
    template<uintptr_t Address>
    class adc_common {
    public:
        using ADC_COMx_CSR = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADC_COMx_CCR = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
        using ADC_COMx_CDR = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0xFFFFFFFF>;
    };
}}}

#endif

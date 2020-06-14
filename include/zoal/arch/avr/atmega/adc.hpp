#ifndef ZOAL_ARCH_AVR_ATMEGA_ADC_HPP
#define ZOAL_ARCH_AVR_ATMEGA_ADC_HPP

#include "../../../mem/reg.hpp"
#include "../adc.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uint32_t Address>
    class adc_mem_model {
    public:
        using ADCx = zoal::mem::reg<Address + 0, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCLx = zoal::mem::reg<Address + 0, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCHx = zoal::mem::reg<Address + 1, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCSRAx = zoal::mem::reg<Address + 2, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCSRBx = zoal::mem::reg<Address + 3, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADMUXx = zoal::mem::reg<Address + 4, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
    };

    template<uintptr_t Address, uint8_t N>
    using adc = ::zoal::arch::avr::adc<Address, N, adc_mem_model<Address>>;
}}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_ADC_HPP
#define ZOAL_ARCH_AVR_ATMEGA_ADC_HPP

#include "../../../mem/reg.hpp"
#include "../adc.hpp"

namespace zoal::arch::avr::atmega {
    template<uint32_t Address>
    class adc_mem_model {
    public:
        using ADCx = mem::reg<Address + 0, mem::reg_io::read_write, uint16_t, 0xFF>;
        using ADCLx = mem::reg<Address + 0, mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCHx = mem::reg<Address + 1, mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCSRAx = mem::reg<Address + 2, mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADCSRBx = mem::reg<Address + 3, mem::reg_io::read_write, uint8_t, 0xFF>;
        using ADMUXx = mem::reg<Address + 4, mem::reg_io::read_write, uint8_t, 0xFF>;
    };

    template<uintptr_t Address, uint8_t N>
    using adc = adc<Address, N, adc_mem_model<Address>>;
}

#endif

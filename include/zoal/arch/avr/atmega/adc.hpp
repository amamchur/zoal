#ifndef ZOAL_ARCH_AVR_ATMEGA_ADC_HPP
#define ZOAL_ARCH_AVR_ATMEGA_ADC_HPP

#include "../adc.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    class adc_mem_model {
    public:
        static constexpr intptr_t ADCx = 0;
        static constexpr intptr_t ADCSRAx = 2;
        static constexpr intptr_t ADCSRBx = 3;
        static constexpr intptr_t ADMUXx = 4;
    };

    template<uintptr_t Address, uint8_t N>
    using adc = ::zoal::arch::avr::adc<Address, N, adc_mem_model>;
}}}}

#endif

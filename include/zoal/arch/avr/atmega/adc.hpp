#ifndef ZOAL_ARCH_AVR_ATMEGA_ADC_HPP
#define ZOAL_ARCH_AVR_ATMEGA_ADC_HPP

#include "../adc.hpp"

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<uintptr_t Address, uint8_t N>
    using adc = ::zoal::arch::avr::adc<zoal::arch::avr::mcu_type::atmega, Address, N>;
}}}}

#endif

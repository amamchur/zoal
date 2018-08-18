#ifndef ZOAL_ARCH_ATMEL_AVR_A2D_Converter_MODEL_HPP
#define ZOAL_ARCH_ATMEL_AVR_A2D_Converter_MODEL_HPP

#include <stdint.h> /* NOLINT */
#include "mcu_type.hpp"

namespace zoal { namespace arch { namespace avr {
    template<mcu_type type>
    class adc_converter_model {
    };

    template<>
    class adc_converter_model<mcu_type::atmega> {
    public:
        static constexpr uintptr_t ADMUXxDef = 1 << 6;

        static constexpr uintptr_t ADCLx = 0;
        static constexpr uintptr_t ADCHx = 1;
        static constexpr uintptr_t ADCSRAx = 2;
        static constexpr uintptr_t ADCSRBx = 3;
        static constexpr uintptr_t ADMUXx = 4;
    };

    template<>
    class adc_converter_model<mcu_type::attiny13> {
    public:
        static constexpr uintptr_t ADMUXxDef = 0;

        static constexpr uintptr_t ADCSRBx = 0;
        static constexpr uintptr_t ADCLx = 1;
        static constexpr uintptr_t ADCHx = 2;
        static constexpr uintptr_t ADCSRAx = 3;
        static constexpr uintptr_t ADMUXx = 4;
    };
}}}

#endif

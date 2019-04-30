#ifndef ZOAL_PERIPH_ADC_CONFIG_HPP
#define ZOAL_PERIPH_ADC_CONFIG_HPP

namespace zoal { namespace periph {
    enum class adc_ref {
        vcc,
        internal_1v1,
        internal_2v56,
        external
    };
}}

#endif

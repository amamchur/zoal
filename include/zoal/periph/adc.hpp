#ifndef ZOAL_PERIPH_ADC_CONFIG_HPP
#define ZOAL_PERIPH_ADC_CONFIG_HPP

namespace zoal { namespace periph {
    enum class adc_ref { vcc, internal_1v1, internal_2v56, external };

    template<adc_ref Ref = adc_ref::vcc>
    class adc_config {
    public:
        static constexpr adc_ref reference = Ref;
    };
}}

#endif

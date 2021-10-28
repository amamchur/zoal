#ifndef ZOAL_PERIPH_ADC_CONFIG_HPP
#define ZOAL_PERIPH_ADC_CONFIG_HPP

namespace zoal { namespace periph {
    enum class adc_ref { vcc, internal_1v1, internal_2v56, external };

    template<uintptr_t ClockDivider = 4, adc_ref Ref = adc_ref::vcc>
    class adc_params {
    public:
        static constexpr uintptr_t clock_divider = ClockDivider;
        static constexpr adc_ref reference = Ref;
    };

    template<class A, class Pin, uint8_t Channel, class Connect, class Disconnect>
    class adc_channel {
    public:
        using adc = A;
        using pin = Pin;
        static constexpr auto channel = Channel;
        using connect = Connect;
        using disconnect = Disconnect;

        static uint16_t read() {
            connect();
            return adc::read();
        }
    };
}}

#endif

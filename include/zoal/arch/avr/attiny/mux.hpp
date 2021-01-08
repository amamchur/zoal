#ifndef ZOAL_ARCH_AVR_ATTINY_MUX_HPP
#define ZOAL_ARCH_AVR_ATTINY_MUX_HPP

#include "../../../gpio/api.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../mem/cas.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<uintptr_t UsartAddress, uint32_t PortAddress, uint8_t PinOffset>
    struct usart_mapping;

    template<class Adc, class Pin>
    struct adc_mapping;

    template<class Timer, class Pin, uint8_t Channel>
    struct pwm_channel_mapping;
}}

namespace zoal { namespace arch { namespace avr { namespace attiny {
    using zoal::metadata::adc_mapping;
    using zoal::metadata::pwm_channel_mapping;
    using zoal::metadata::usart_mapping;

    template<class Api>
    class mux {
    public:
        template<class A, class Pin>
        class adc {
        public:
            static constexpr auto channel = adc_mapping<A, Pin>::value;
            static constexpr auto mux_set_mask = channel & 0x07;
            static constexpr auto mux5 = channel > 7 ? (1 << 3) : 0;

            static_assert(channel >= 0, "Specified pin could not be connected to ADC");

            using connect = zoal::mem::cas_list<
                //
                typename A::ADMUXx::template cas<0x0F, mux_set_mask>,
                typename A::ADCSRBx::template cas<(1 << 3), mux5>>;

            static void on() {
                typename A::ADMUXx::template cas<0x0F, mux_set_mask>();
                typename A::ADCSRBx::template cas<(1 << 3), mux5>();
            }

            static void off() {}
        };

        template<class T, class Pin, uint8_t Channel>
        class timer {
        public:
            static_assert(Channel < 2, "Channel index if out of range");
            static_assert(pwm_channel_mapping<T, Pin, Channel>::value, "Unsupported PWM connection");

            static constexpr auto mask = static_cast<uint8_t>(Channel == 0 ? (1u << 7u) : (1u << 5u));
            using TCCRxA_cfg_on = typename T::TCCRxA::template cas<0, mask>;
            using TCCRxA_cfg_off = typename T::TCCRxA::template cas<mask, 0>;

            using connect = zoal::mem::cas_list<TCCRxA_cfg_on>;
            using disconnect = zoal::mem::cas_list<TCCRxA_cfg_off>;
        };
    };
}}}}

#endif

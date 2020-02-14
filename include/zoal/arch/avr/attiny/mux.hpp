#ifndef ZOAL_ARCH_AVR_ATTINY_MUX_HPP
#define ZOAL_ARCH_AVR_ATTINY_MUX_HPP

#include "../../../gpio/pin.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/accessor.hpp"
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
    using zoal::mem::clear_and_set;
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

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, A::address, Offset>;

            static void on() {
                clear_and_set<0x0F, mux_set_mask>::apply(accessor<A::ADMUXx>::ref());
                clear_and_set<(1 << 3), mux5>::apply(accessor<A::ADCSRBx>::ref());
            }

            static void off() {}
        };

        template<class T, class Pin, uint8_t Channel>
        class timer {
        public:
            static_assert(Channel < 2, "Channel index if out of range");
            static_assert(pwm_channel_mapping<T, Pin, Channel>::value, "Unsupported PWM connection");

            static constexpr auto mask = static_cast<uint8_t>(Channel == 0 ? (1u << 7u) : (1u << 5u));
            using TCCRxA_cfg_on = clear_and_set<0, mask>;
            using TCCRxA_cfg_off = clear_and_set<mask, 0>;

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, T::address, Offset>;

            static void on() {
                TCCRxA_cfg_on::apply(accessor<T::TCCRxA>::ref());
            }

            static void off() {
                TCCRxA_cfg_off::apply(accessor<T::TCCRxA>::ref());
            }
        };
    };
}}}}

#endif

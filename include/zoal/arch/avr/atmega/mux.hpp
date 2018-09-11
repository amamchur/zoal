#ifndef ZOAL_ARCH_AVR_MUX_HPP
#define ZOAL_ARCH_AVR_MUX_HPP

#include "../../../gpio/pin.hpp"
#include "../../../utils/helpers.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace metadata {
    template<uintptr_t UsartAddress, uint32_t PortAddress, uint8_t PinOffset>
    struct usart_mapping;

    template<class Adc, class Pin>
    struct adc_mapping;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template<class Api>
    class mux {
    public:
        template<class U, class Rx, class Tx, class Ck = zoal::gpio::null_pin>
        class usart {
        public:
            using rxm = zoal::metadata::usart_mapping<U::address, Rx::port::address, Rx::offset>;
            using txm = zoal::metadata::usart_mapping<U::address, Tx::port::address, Tx::offset>;
            using ckm = zoal::metadata::usart_mapping<U::address, Ck::port::address, Ck::offset>;

            static_assert(rxm::rx >= 0, "Specified RX pin could not be connected to USART");
            static_assert(txm::tx >= 0, "Specified TX pin could not be connected to USART");
            static_assert(ckm::ck >= 0, "Specified CX pin could not be connected to USART");

            static void on() {}
        };

        template<class A, class Pin>
        class adc {
        public:
            static constexpr auto channel = zoal::metadata::adc_mapping<A, Pin>::value;
            static constexpr auto mux_set_mask = (channel & 0x07) | (1 << 6);
            static constexpr auto mux5 = channel > 7 ? (1 << 3) : 0;

            static_assert(channel >= 0, "Specified pin could not be connected to ADC");

            static void on() {
                zoal::utils::memory_segment<uint8_t, A::address> mem;
                zoal::utils::clear_and_set<0x0F, mux_set_mask>::apply(mem[A::ADMUXx]);
                zoal::utils::clear_and_set<(1 << 3), mux5>::apply(mem[A::ADCSRBx]);
            }
        };
    };
}}}}

#endif

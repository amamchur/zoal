#ifndef ZOAL_ARCH_AVR_ATMEGA_MUX_HPP
#define ZOAL_ARCH_AVR_ATMEGA_MUX_HPP

#include "../../../gpio/pin.hpp"

namespace zoal { namespace metadata {
    template<uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct atmega_usart_mapping;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template <class Api>
    class mux {
    public:
        template<
                class U,
                class Rx,
                class Tx,
                class Ck = zoal::gpio::null_pin>
        class usart {
        public:
            using txm = zoal::metadata::atmega_usart_mapping<U::address, Tx::port::address, Tx::offset>;
            using rxm = zoal::metadata::atmega_usart_mapping<U::address, Rx::port::address, Rx::offset>;
            using ckm = zoal::metadata::atmega_usart_mapping<U::address, Ck::port::address, Ck::offset>;

            static_assert(txm::tx >= 0, "Unsupported TX pin mapping");
            static_assert(rxm::rx >= 0, "Unsupported RX pin mapping");
            static_assert(ckm::ck >= 0, "Unsupported CK pin mapping");

            static inline void on() {
            }
        };
    };
}}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_MUX_HPP
#define ZOAL_ARCH_AVR_ATMEGA_MUX_HPP

#include "../../../gpio/pin.hpp"

namespace zoal { namespace metadata {
    template<uint8_t Usart, uint32_t Port, uint8_t PinOffset>
    struct atmega_usart_mapping;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    template <class Api>
    class mux {
    public:
        template<
                class U,
                class PinRX,
                class PinTX,
                class PinCK = zoal::gpio::null_pin>
        class usart {
        public:
            using tx_af = zoal::metadata::atmega_usart_mapping<U::no, PinTX::port::address, PinTX::offset>;
            using rx_af = zoal::metadata::atmega_usart_mapping<U::no, PinRX::port::address, PinRX::offset>;
            using ck_af = zoal::metadata::atmega_usart_mapping<U::no, PinCK::port::address, PinCK::offset>;

            static_assert(tx_af::tx >= 0, "Unsupported TX pin mapping");
            static_assert(rx_af::rx >= 0, "Unsupported RX pin mapping");
            static_assert(ck_af::ck >= 0, "Unsupported CK pin mapping");

            static inline void on() {
            }
        };
    };
}}}}

#endif

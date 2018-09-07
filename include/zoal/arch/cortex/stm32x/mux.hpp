#ifndef ZOAL_ARCH_STM32X_MUX_HPP
#define ZOAL_ARCH_STM32X_MUX_HPP

#include <stdint.h>
#include "../../../gpio/pin.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<uint8_t Usart, uint32_t Port, uint8_t PinOffset>
    struct stm32_usart_af_mapping;
}}

namespace zoal { namespace arch { namespace stm32x {
    template <class Api>
    class stm32x_mux {
    public:
        template<
                class U,
                class PinTX = zoal::gpio::null_pin,
                class PinRX = zoal::gpio::null_pin,
                class PinCK = zoal::gpio::null_pin>
        class usart {
        public:
            using tx_af = zoal::metadata::stm32_usart_af_mapping<U::no, PinTX::port::address, PinTX::offset>;
            using rx_af = zoal::metadata::stm32_usart_af_mapping<U::no, PinRX::port::address, PinRX::offset>;
            using ck_af = zoal::metadata::stm32_usart_af_mapping<U::no, PinCK::port::address, PinCK::offset>;

            static_assert(tx_af::tx >= 0, "Unsupported TX pin mapping");
            static_assert(rx_af::rx >= 0, "Unsupported RX pin mapping");
            static_assert(ck_af::ck >= 0, "Unsupported CK pin mapping");

            static inline void on() {
                using namespace zoal::utils;
                Api::template power_on<PinTX, PinRX, PinCK>::apply();

                if (is_pin<PinTX>::value) {
                    PinTX::port::template stm32_alternate_function<PinTX::offset, tx_af::tx>();
                }

                if (is_pin<PinRX>::value) {
                    PinTX::port::template stm32_alternate_function<PinRX::offset, rx_af::rx>();
                }

                if (is_pin<PinCK>::value) {
                    PinTX::port::template stm32_alternate_function<PinCK::offset, ck_af::ck>();
                }
            }
        };
    };
}}}

#endif


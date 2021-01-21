#ifndef ZOAL_ARCH_STM32X_MUX_HPP
#define ZOAL_ARCH_STM32X_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../ct/helpers.hpp"
#include "metadata.hpp"

#include <stdint.h>

namespace zoal { namespace metadata {
    template<uintptr_t Usart, uintptr_t Port, uint8_t PinOffset>
    struct stm32_usart_af_mapping;

    template<class Sign, uintptr_t A, uintptr_t Port, uint8_t Pin, signal s>
    struct stm32_af;
}}

namespace zoal { namespace arch { namespace stm32x {
    template<class Microcontroller>
    class mux {
    public:
        using mcu = Microcontroller;
        using sign = typename mcu::signature;

        template<class U, class PinRX, class PinTX>
        class usart {
        public:
            using rx_af = zoal::metadata::stm32_af<sign, U::address, PinRX::port::address, PinRX::offset, zoal::metadata::signal::rx>;
            using tx_af = zoal::metadata::stm32_af<sign, U::address, PinTX::port::address, PinTX::offset, zoal::metadata::signal::tx>;

            static_assert(rx_af::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_af::value >= 0, "Unsupported TX pin mapping");

            static inline void on() {
                using namespace zoal::ct;
                stm32_alternate_function<typename PinTX::port, PinTX::offset, tx_af::value>();
                stm32_alternate_function<typename PinTX::port, PinRX::offset, rx_af::value>();
            }

        private:
            template<class Port, uint8_t Pin, uint8_t af>
            static inline void stm32_alternate_function() {
                using reg = typename zoal::ct::conditional_type<Pin < 8, typename Port::GPIOx_AFRL, typename Port::GPIOx_AFRH>::type;
                static const auto shift = (Pin & 0x7) << 2;
                using list = zoal::mem::callable_cas_list_variadic<
                    typename Port::GPIOx_OSPEEDR::template cas<0, 0x3 << (Pin * 2)>,
                    typename Port::GPIOx_OTYPER::template cas<0x1 << Pin, 0>,
                    typename Port::GPIOx_MODER::template cas<0x3 << (Pin * 2), 0x2 << (Pin * 2)>,
                    typename reg::template cas<0xF << shift, af << shift>
                    >;
                zoal::mem::apply_cas_list<list>::apply();
            }
        };
    };
}}}

#endif

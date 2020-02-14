#ifndef ZOAL_ARCH_STM32X_MUX_HPP
#define ZOAL_ARCH_STM32X_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../utils/helpers.hpp"
#include "metadata.hpp"

#include <stdint.h>

namespace zoal { namespace metadata {
    template<uintptr_t Usart, uintptr_t Port, uint8_t PinOffset>
    struct stm32_usart_af_mapping;

    template<uintptr_t A, uintptr_t Port, uint8_t Pin, signal s>
    struct stm32_af;
}}

namespace zoal { namespace arch { namespace stm32x {
    template<class Microcontroller>
    class mux {
    public:
        using mcu = Microcontroller;
        using api = typename mcu::api;

        template<class U,
                class PinRX,
                class PinTX>
        class usart {
        public:
//            using rx_af = zoal::metadata::stm32_usart_af_mapping<U::address, PinRX::port::address, PinRX::offset>;
//            using tx_af = zoal::metadata::stm32_usart_af_mapping<U::address, PinTX::port::address, PinTX::offset>;
//            using ck_af = zoal::metadata::stm32_usart_af_mapping<U::address, PinCK::port::address, PinCK::offset>;

            using rx_af = zoal::metadata::stm32_af<U::address, PinRX::port::address, PinRX::offset, zoal::metadata::signal::rx>;
            using tx_af = zoal::metadata::stm32_af<U::address, PinTX::port::address, PinTX::offset, zoal::metadata::signal::tx>;

            static_assert(rx_af::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_af::value >= 0, "Unsupported TX pin mapping");

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint32_t, U::address, Offset>;

            usart() = delete;

            static inline void on() {
                using namespace zoal::ct;
                stm32_alternate_function<typename PinTX::port, PinTX::offset, tx_af::value>();
                stm32_alternate_function<typename PinTX::port, PinRX::offset, rx_af::value>();
            }

        private:
            template<class Port, uint8_t Pin, uint8_t af>
            static inline void stm32_alternate_function() {
                Port::template accessor<Port::GPIOx_OSPEEDR>::ref() |= (0x3 << (Pin * 2)); // 50MHz
                Port::template accessor<Port::GPIOx_OTYPER>::ref() &= ~(0x1 << Pin); // Output push-pull
                zoal::mem::clear_and_set<0x3, 0x2, Pin * 2>::apply(Port::template accessor<Port::GPIOx_MODER>::ref());

                constexpr auto index = Pin < 8 ? Port::GPIOx_AFRL : Port::GPIOx_AFRH;
                zoal::mem::clear_and_set<0xF, af, (Pin & 0x7) << 2>::apply(Port::template accessor<index>::ref());
            }
        };
    };
}}}

#endif

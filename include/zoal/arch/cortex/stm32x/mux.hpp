#ifndef ZOAL_ARCH_STM32X_MUX_HPP
#define ZOAL_ARCH_STM32X_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../mem/clear_and_set.hpp"
#include "../../../utils/helpers.hpp"

#include <stdint.h>

namespace zoal { namespace metadata {
    template<uintptr_t Usart, uintptr_t Port, uint8_t PinOffset>
    struct stm32_usart_af_mapping;
}}

namespace zoal { namespace arch { namespace stm32x {
    template<class Microcontroller>
    class mux {
    public:
        using mcu = Microcontroller;
        using api = typename mcu::api;

        template<class U,
                 class PinRX = zoal::gpio::null_pin,
                 class PinTX = zoal::gpio::null_pin,
                 class PinCK = zoal::gpio::null_pin>
        class usart {
        public:
            using rx_af = zoal::metadata::stm32_usart_af_mapping<U::address, PinRX::port::address, PinRX::offset>;
            using tx_af = zoal::metadata::stm32_usart_af_mapping<U::address, PinTX::port::address, PinTX::offset>;
            using ck_af = zoal::metadata::stm32_usart_af_mapping<U::address, PinCK::port::address, PinCK::offset>;

            static_assert(rx_af::rx >= 0, "Unsupported RX pin mapping");
            static_assert(tx_af::tx >= 0, "Unsupported TX pin mapping");
            static_assert(ck_af::ck >= 0, "Unsupported CK pin mapping");

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint32_t, U::address, Offset>;

            static inline void on() {
                using namespace zoal::ct;

                if (is_pin<PinTX>::value) {
                    stm32_alternate_function<typename PinTX::port, PinTX::offset, tx_af::tx>();
                }

                if (is_pin<PinRX>::value) {
                    stm32_alternate_function<typename PinTX::port, PinRX::offset, rx_af::rx>();
                }

                if (is_pin<PinCK>::value) {
                    stm32_alternate_function<typename PinTX::port, PinCK::offset, ck_af::ck>();
                }
            }

        private:
            template<class Port, uint8_t Pin, uint8_t af>
            static inline void stm32_alternate_function() {
                using namespace zoal::mem;

                *accessor<Port::GPIOx_OSPEEDR>::p |= (0x3 << (Pin * 2)); // 50MHz
                *accessor<Port::GPIOx_OTYPER>::p &= ~(0x1 << Pin); // Output push-pull
                clear_and_set<0x3, 0x2, Pin * 2>::apply(*accessor<Port::GPIOx_MODER>::p);

                constexpr auto index = Pin < 8 ? Port::GPIOx_AFRL : Port::GPIOx_AFRH;
                clear_and_set<0xF, af, (Pin & 0x7) << 2>::apply(*accessor<index>::p);
            }
        };
    };
}}}

#endif

#ifndef ZOAL_ARCH_STM32X_MUX_HPP
#define ZOAL_ARCH_STM32X_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../ct/helpers.hpp"
#include "metadata.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32x {
    using zoal::gpio::api;
    using zoal::gpio::pin_mode;

    template<class Microcontroller>
    class mux {
    public:
        using mcu = Microcontroller;
        using sign = typename mcu::signature;

        template<class U, class PinRX, class PinTX>
        class usart {
        private:
            template<class Port, uint8_t Pin, uint8_t af>
            class stm32_alternate_function_mux {
            public:
                using af_reg = typename zoal::ct::conditional_type <(Pin < 8), typename Port::GPIOx_AFRL, typename Port::GPIOx_AFRH>::type;
                static constexpr uint32_t af_shift = (Pin & 0x7) << 2;
                static constexpr uint32_t af_clear = 0xF << af_shift;
                static constexpr uint32_t af_set = af << af_shift;

                using list = zoal::mem::callable_cas_list_variadic<typename Port::GPIOx_MODER::template cas<0x3 << (Pin * 2), 0x2 << (Pin * 2)>,
                                                                   typename Port::GPIOx_OTYPER::template cas<0x1 << Pin, 0>,
                                                                   typename Port::GPIOx_OSPEEDR::template cas<0, 0x3 << (Pin * 2)>,
                                                                   typename af_reg::template cas<af_clear, af_set>>;
                using connect_cas = api::optimize<list>;
            };

        public:
            using rx_af = zoal::metadata::stm32_signal_map<sign, U::address, PinRX::port::address, PinRX::offset, zoal::metadata::signal::rx>;
            using tx_af = zoal::metadata::stm32_signal_map<sign, U::address, PinTX::port::address, PinTX::offset, zoal::metadata::signal::tx>;

            static_assert(rx_af::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_af::value >= 0, "Unsupported TX pin mapping");

            // Enable bus clock for specified ports
            using clock_tx_on = typename PinRX::port::clock_on_cas;
            using clock_rx_on = typename PinRX::port::clock_on_cas;

            // Final merge
            using periph_clock_on = api::optimize<clock_tx_on, clock_rx_on>;
            using connect = api::optimize<
                //
                typename stm32_alternate_function_mux<typename PinRX::port, PinRX::offset, rx_af::value>::connect_cas,
                typename stm32_alternate_function_mux<typename PinTX::port, PinTX::offset, tx_af::value>::connect_cas>;
            using disconnect = api::optimize<api::mode<pin_mode::input, PinTX, PinTX>>;
        };
    };
}}}

#endif

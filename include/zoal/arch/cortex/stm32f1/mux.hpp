#ifndef ZOAL_ARCH_STM32F1_MUX_HPP
#define ZOAL_ARCH_STM32F1_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../gpio/api.hpp"
#include "../../../utils/helpers.hpp"
#include "../stm32x/metadata.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {
    using zoal::ct::type_list;
    using zoal::metadata::signal;

    template<class Microcontroller>
    class mux {
    private:
        using afio = typename Microcontroller::afio;
    public:
        template<class U, class PinRX, class PinTX>
        class usart {
        public:
            using rx_rm = zoal::metadata::stm32_remap<U::address, PinRX::port::address, PinRX::offset, signal::rx>;
            using tx_rm = zoal::metadata::stm32_remap<U::address, PinTX::port::address, PinTX::offset, signal::tx>;

            static_assert(rx_rm::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_rm::value >= 0, "Unsupported TX pin mapping");
            static_assert(rx_rm::value == tx_rm::value, "Different remapping");

            using rx_in = typename PinRX::port::template mode_cas<zoal::gpio::pin_mode::input_floating, PinRX::mask>;
            using tx_af = typename PinTX::port::template port_cnf_mode<0xB, PinTX::mask>::all;
            using tx_in = typename PinTX::port::template mode_cas<zoal::gpio::pin_mode::input_floating, PinTX::mask>;

            using connect = zoal::gpio::api::optimize<rx_in, tx_af>;
            using disconnect = zoal::gpio::api::optimize<rx_in, tx_in>;
        };
    };
}}}

#endif

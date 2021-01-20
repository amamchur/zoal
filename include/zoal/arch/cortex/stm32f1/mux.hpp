#ifndef ZOAL_ARCH_STM32F1_MUX_HPP
#define ZOAL_ARCH_STM32F1_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../gpio/api.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../utils/helpers.hpp"
#include "../stm32x/metadata.hpp"

#include <stdint.h>

namespace zoal { namespace arch { namespace stm32f1 {
    using zoal::ct::type_list;
    using zoal::metadata::signal;
    using zoal::metadata::stm32_af_to_remap_mode;
    using zoal::metadata::stm32_afio_mapr;
    using zoal::metadata::stm32_afio_mapr2;
    using zoal::metadata::stm32_remap_mode;

    template<class Microcontroller>
    class mux {
    private:
        using afio = typename Microcontroller::afio;

    public:
        template<class U, class PinRX, class PinTX>
        class usart {
        public:
            // Test remapping allowed
            using rx_rm = zoal::metadata::stm32_remap<U::address, PinRX::port::address, PinRX::offset, signal::rx>;
            using tx_rm = zoal::metadata::stm32_remap<U::address, PinTX::port::address, PinTX::offset, signal::tx>;
            static_assert(rx_rm::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_rm::value >= 0, "Unsupported TX pin mapping");

            // Configuring pin remapping
            static constexpr stm32_remap_mode mode = stm32_af_to_remap_mode<rx_rm::value, tx_rm::value>::value;
            using afio_mapr = stm32_afio_mapr<U::address, mode>;
            using afio_mapr2 = stm32_afio_mapr2<U::address, mode>;
            using afio_mapr_connect_cas = typename afio::AFIOx_MAPR::template cas<afio_mapr::clear, afio_mapr::set>;
            using afio_mapr2_connect_cas = typename afio::AFIOx_MAPR2::template cas<afio_mapr2::clear, afio_mapr2::set>;
            using afio_mapr_disconnect_cas = typename afio::AFIOx_MAPR::template cas<afio_mapr::clear, 0>;
            using afio_mapr2_disconnect_cas = typename afio::AFIOx_MAPR2::template cas<afio_mapr2::clear, 0>;
            using afio_connect = zoal::ct::type_list<afio_mapr_connect_cas, afio_mapr2_connect_cas>;
            using afio_disconnect = zoal::ct::type_list<afio_mapr_disconnect_cas, afio_mapr2_disconnect_cas>;

            // Configuring pin to alternative mode
            using rx_in = typename PinRX::port::template mode_cas<zoal::gpio::pin_mode::input_floating, PinRX::mask>;
            using tx_af = typename PinTX::port::template port_cnf_mode<0xB, PinTX::mask>::all;
            using tx_in = typename PinTX::port::template mode_cas<zoal::gpio::pin_mode::input_floating, PinTX::mask>;

            // Enable bus clock for specified ports
            using clock_tx_on = typename PinRX::port::clock_on_cas;
            using clock_rx_on = typename PinRX::port::clock_on_cas;

            // Final marge
            using periph_clock_on = zoal::gpio::api::optimize<clock_tx_on, clock_rx_on>;
            using connect = zoal::gpio::api::optimize<clock_tx_on, clock_rx_on, afio_connect, rx_in, tx_af>;
            using disconnect = zoal::gpio::api::optimize<rx_in, tx_in, afio_disconnect>;
        };
    };
}}}

#endif

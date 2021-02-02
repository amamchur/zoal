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
    public:
        using mcu = Microcontroller;
        using sign = typename mcu::signature;
        using afio = typename mcu::afio;

        template<class U, class PinRX, class PinTX>
        class usart {
        public:
            // Test remapping allowed
            using rx_rm = zoal::metadata::stm32_signal_map<sign, U::address, PinRX::port::address, PinRX::offset, signal::rx>;
            using tx_rm = zoal::metadata::stm32_signal_map<sign, U::address, PinTX::port::address, PinTX::offset, signal::tx>;
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
            using rx_in_fl = typename PinRX::port::template mode_cas<zoal::gpio::pin_mode::input_floating, PinRX::mask>;
            using tx_af_pp = typename PinTX::port::template port_cnf_mode<0xB, PinTX::mask>::all;
            using tx_in_fl = typename PinTX::port::template mode_cas<zoal::gpio::pin_mode::input_floating, PinTX::mask>;

            // Enable bus clock for specified ports
            using clock_tx_on = typename PinRX::port::clock_on_cas;
            using clock_rx_on = typename PinRX::port::clock_on_cas;

            // Final merge
            using periph_clock_on = zoal::gpio::api::optimize<clock_tx_on, clock_rx_on>;
            using connect = zoal::gpio::api::optimize<afio_connect, rx_in_fl, tx_af_pp>;
            using disconnect = zoal::gpio::api::optimize<rx_in_fl, tx_in_fl, afio_disconnect>;
        };

        template<class I, class SdaPin, class SclPin>
        class i2c {
        public:
            using sda_rm = zoal::metadata::stm32_signal_map<sign, I::address, SdaPin::port::address, SdaPin::offset, signal::sda>;
            using scl_rm = zoal::metadata::stm32_signal_map<sign, I::address, SclPin::port::address, SclPin::offset, signal::scl>;

            static_assert(sda_rm::value >= 0, "Unsupported SDA pin mapping");
            static_assert(scl_rm::value >= 0, "Unsupported SCL pin mapping");

            // Configuring pin remapping
//            static constexpr stm32_remap_mode mode = stm32_af_to_remap_mode<sda_rm::value, scl_rm::value>::value;
//            using afio_mapr = stm32_afio_mapr<I::address, mode>;
//            using afio_mapr2 = stm32_afio_mapr2<I::address, mode>;
//            using afio_mapr_connect_cas = typename afio::AFIOx_MAPR::template cas<afio_mapr::clear, afio_mapr::set>;
//            using afio_mapr2_connect_cas = typename afio::AFIOx_MAPR2::template cas<afio_mapr2::clear, afio_mapr2::set>;
//            using afio_mapr_disconnect_cas = typename afio::AFIOx_MAPR::template cas<afio_mapr::clear, 0>;
//            using afio_mapr2_disconnect_cas = typename afio::AFIOx_MAPR2::template cas<afio_mapr2::clear, 0>;
//            using afio_connect = zoal::ct::type_list<afio_mapr_connect_cas, afio_mapr2_connect_cas>;
//            using afio_disconnect = zoal::ct::type_list<afio_mapr_disconnect_cas, afio_mapr2_disconnect_cas>;

            // Configuring pin to alternative mode
            using sda_in_fl = typename SdaPin::port::template mode_cas<zoal::gpio::pin_mode::input_floating, SdaPin::mask>;
            using sda_af_od = typename SdaPin::port::template port_cnf_mode<0xF, SdaPin::mask>::all;
            using scl_in_fl = typename SclPin::port::template mode_cas<zoal::gpio::pin_mode::input_floating, SclPin::mask>;
            using scl_af_od = typename SclPin::port::template port_cnf_mode<0xF, SclPin::mask>::all;

            using clock_sda_on = typename SdaPin::port::clock_on_cas;
            using clock_scl_on = typename SclPin::port::clock_on_cas;

            using periph_clock_on = zoal::gpio::api::optimize<clock_sda_on, clock_scl_on>;
            using connect = api::optimize<sda_af_od, scl_af_od>;
            using disconnect = api::optimize<sda_in_fl, scl_in_fl>;
        };
    };
}}}

#endif

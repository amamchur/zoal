#ifndef ZOAL_ARCH_STM32F4_MUX_HPP
#define ZOAL_ARCH_STM32F4_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../ct/helpers.hpp"
#include "../../../gpio/api.hpp"
#include "../../../gpio/pin_mode.hpp"
#include "../../../periph/adc.hpp"
#include "../../../periph/pwm.hpp"
#include "../stm32x/metadata.hpp"

#include <cstdint>

namespace zoal { namespace arch { namespace stm32f4 {
    using zoal::gpio::api;
    using zoal::gpio::pin_mode;
    using zoal::metadata::signal;

    template<class Pin, uint8_t af, uint8_t ot>
    class stm32_alternate_function_mux {
    public:
        using port = typename Pin::port;
        static constexpr auto pin_offset = Pin::offset;

        using af_reg = typename zoal::ct::conditional_type<(pin_offset < 8), typename port::GPIOx_AFRL, typename port::GPIOx_AFRH>::type;
        static constexpr uint32_t af_shift = (pin_offset & 0x7u) << 2;
        static constexpr uint32_t af_clear = 0xFu << af_shift;
        static constexpr uint32_t af_set = af << af_shift;

        using list = zoal::mem::callable_cas_list_variadic<typename port::GPIOx_MODER::template cas<0x3u << (pin_offset * 2), 0x2u << (pin_offset * 2)>,
                                                           typename port::GPIOx_OTYPER::template cas<0x1u << pin_offset, ot << pin_offset>,
                                                           typename port::GPIOx_OSPEEDR::template cas<0u, 0x3u << (pin_offset * 2)>,
                                                           typename port::GPIOx_PUPDR::template cas<0x3u << (pin_offset * 2), 0>,
                                                           typename af_reg::template cas<af_clear, af_set>>;
        using connect_cas = api::optimize<list>;
    };

    class valid_signal {
    public:
        template<class T>
        class match {
        public:
            static constexpr bool value = T::value >= 0;
        };
    };

    template<class Sign, class A, class Pin>
    class adc_channel_builder {
    public:
        using ch0_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in0>;
        using ch1_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in1>;
        using ch2_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in2>;
        using ch3_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in3>;
        using ch4_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in4>;
        using ch5_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in5>;
        using ch6_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in6>;
        using ch7_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in7>;
        using ch8_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in8>;
        using ch9_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in9>;
        using ch10_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in10>;
        using ch11_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in11>;
        using ch12_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in12>;
        using ch13_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in13>;
        using ch14_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in14>;
        using ch15_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in15>;
        using ch16_map = zoal::metadata::stm32_signal_map<Sign, A::address, Pin::port::address, Pin::offset, signal::in16>;
        using ch_maps = zoal::ct::type_list<
            //
            ch0_map,
            ch1_map,
            ch2_map,
            ch3_map,
            ch4_map,
            ch5_map,
            ch6_map,
            ch7_map,
            ch8_map,
            ch9_map,
            ch10_map,
            ch11_map,
            ch12_map,
            ch13_map,
            ch14_map,
            ch15_map,
            ch16_map>;

        static constexpr int channel = zoal::ct::index_of_type<valid_signal, ch_maps>::result;
        static_assert(channel >= 0, "Specified pin could not be connected to ADC");

        using smprs = zoal::ct::type_list<typename A::ADCx_SMPR2, typename A::ADCx_SMPR1>;
        using smpr = typename zoal::ct::type_at_index<channel / 10, void, smprs>::result;
        using sqr3_cas = typename A::ADCx_SQR3::template cas<0xF, channel>;

        using port = typename Pin::port;
        using moder_cas = typename port::GPIOx_MODER::template cas<(0x3 << (Pin::offset * 2)), 0x3 << (Pin::offset * 2)>;

        using connect = typename api::optimize<
            //
            zoal::mem::callable_cas_list_variadic<moder_cas, sqr3_cas>>;
        using disconnect = typename api::optimize<
            //
            api::mode<pin_mode::input_floating, Pin>>;

        using result = zoal::periph::adc_channel<A, Pin, channel, connect, disconnect>;
    };

    template<class Sign, class T, class Pin>
    class pwm_channel_builder {
    public:
        using ch1_map = zoal::metadata::stm32_signal_map<Sign, T::address, Pin::port::address, Pin::offset, signal::ch1>;
        using ch2_map = zoal::metadata::stm32_signal_map<Sign, T::address, Pin::port::address, Pin::offset, signal::ch2>;
        using ch3_map = zoal::metadata::stm32_signal_map<Sign, T::address, Pin::port::address, Pin::offset, signal::ch3>;
        using ch4_map = zoal::metadata::stm32_signal_map<Sign, T::address, Pin::port::address, Pin::offset, signal::ch4>;
        using ch_maps = zoal::ct::type_list<ch1_map, ch2_map, ch3_map, ch3_map>;

        static constexpr int channel = zoal::ct::index_of_type<valid_signal, ch_maps>::result;
        static_assert(channel >= 0, "Specified pin could not be connected to Timer");

        static constexpr int af = zoal::ct::type_at_index<channel, void, ch_maps>::result::value;
        using port = typename Pin::port;
        using afrs = zoal::ct::type_list<typename port::GPIOx_AFRL, typename port::GPIOx_AFRH>;
        using afr = typename zoal::ct::type_at_index<Pin::offset / 8, void, afrs>::result;
        static constexpr uint32_t af_shift = (Pin::offset & 0x7) << 2;
        static constexpr uint32_t af_clear = 0xF << af_shift;
        static constexpr uint32_t af_set = af << af_shift;

        static constexpr uint32_t moder_clear = static_cast<uint32_t>(0x3u << (Pin::offset * 2));
        static constexpr uint32_t moder_set = static_cast<uint32_t>(0x2u << (Pin::offset * 2));
        using moder_cas = typename port::GPIOx_MODER::template cas<moder_clear, moder_set>;
        using afr_cas = typename afr::template cas<af_clear, af_set>;

        using ccrs = zoal::ct::type_list<
            //
            typename T::TIMERx_CCR1,
            typename T::TIMERx_CCR2,
            typename T::TIMERx_CCR3,
            typename T::TIMERx_CCR4>;
        using ccr = typename zoal::ct::type_at_index<channel, void, ccrs>::result;

        static constexpr int channel_ccmr_group = channel >> 1;
        using ccmrs = zoal::ct::type_list<typename T::TIMERx_CCMR1, typename T::TIMERx_CCMR2>;
        using ccmr = typename zoal::ct::type_at_index<channel_ccmr_group, void, ccmrs>::result;

        static constexpr uint32_t ccmr_shift = channel_ccmr_group == 0 ? 0 : 8;
        static constexpr uint32_t ccmr_ocx_ce = 0x0 << 7;
        static constexpr uint32_t ccmr_ocx_ocm_set = 0x6 << 4; // PWM Mode 1
        static constexpr uint32_t ccmr_ocx_pe = 0x1 << 3;
        static constexpr uint32_t ccmr_ocx_fe = 0x0 << 2;
        static constexpr uint32_t ccmr_ocx_ccs_set = 0x0 << 0;
        static constexpr uint32_t ccmr_connect_clear = 0xFF;
        static constexpr uint32_t ccmr_connect_set = ccmr_ocx_ce | ccmr_ocx_pe | ccmr_ocx_ocm_set | ccmr_ocx_fe | ccmr_ocx_ccs_set;

        static constexpr uint32_t ccer_shift = channel;
        static constexpr uint32_t ccer_clear = 0x0F;
        static constexpr uint32_t ccer_set = 0x01;

        using ccmr_connect_cas = typename ccmr::template cas<(ccmr_connect_clear << ccmr_shift), (ccmr_connect_set << ccmr_shift)>;
        using ccer_connect_cas = typename T::TIMERx_CCER::template cas<(ccer_clear << ccer_shift), (ccer_set << ccer_shift)>;

        using ccmr_disconnect_cas = typename ccmr::template cas<(ccmr_connect_clear << ccmr_shift), 0>;
        using ccer_disconnect_cas = typename T::TIMERx_CCER::template cas<(ccer_clear << ccer_shift), 0>;

        using connect = typename api::optimize<
            //
            zoal::mem::callable_cas_list_variadic<ccmr_connect_cas, ccer_connect_cas, afr_cas, moder_cas>>;

        using disconnect = typename api::optimize<
            //
            zoal::mem::callable_cas_list_variadic<ccer_disconnect_cas>>;

        using result = zoal::periph::pwm_channel<T, Pin, channel, ccr, connect, disconnect>;
    };

    template<class Microcontroller>
    class mux {
    public:
        using mcu = Microcontroller;
        using sign = typename mcu::signature;

        template<class U, class PinRX, class PinTX>
        class usart {
        private:
        public:
            using rx_af = zoal::metadata::stm32_signal_map<sign, U::address, PinRX::port::address, PinRX::offset, zoal::metadata::signal::rx>;
            using tx_af = zoal::metadata::stm32_signal_map<sign, U::address, PinTX::port::address, PinTX::offset, zoal::metadata::signal::tx>;

            static_assert(rx_af::value >= 0, "Unsupported RX pin mapping");
            static_assert(tx_af::value >= 0, "Unsupported TX pin mapping");

            using clock_tx_on = typename PinRX::port::clock_on_cas;
            using clock_rx_on = typename PinRX::port::clock_on_cas;

            using clock_on = api::optimize<clock_tx_on, clock_rx_on>;
            using connect = api::optimize<
                //
                typename stm32_alternate_function_mux<PinRX, rx_af::value, 0>::connect_cas,
                typename stm32_alternate_function_mux<PinTX, tx_af::value, 0>::connect_cas>;
            using disconnect = api::optimize<api::mode<pin_mode::input, PinTX, PinTX>>;
        };

        template<class S, class Miso, class Mosi, class Sclk>
        class spi {
        public:
            using miso_af = zoal::metadata::stm32_signal_map<sign, S::address, Miso::port::address, Miso::offset, zoal::metadata::signal::miso>;
            using mosi_af = zoal::metadata::stm32_signal_map<sign, S::address, Mosi::port::address, Mosi::offset, zoal::metadata::signal::mosi>;
            using sclk_af = zoal::metadata::stm32_signal_map<sign, S::address, Sclk::port::address, Sclk::offset, zoal::metadata::signal::sck>;

            static_assert(miso_af::value >= 0, "Unsupported Miso pin mapping");
            static_assert(mosi_af::value >= 0, "Unsupported Mosi pin mapping");
            static_assert(sclk_af::value >= 0, "Unsupported Sclk pin mapping");

            using clock_miso_on = typename Miso::port::clock_on_cas;
            using clock_mosi_on = typename Mosi::port::clock_on_cas;
            using clock_sclk_on = typename Sclk::port::clock_on_cas;

            using connect = api::optimize<
                //
                typename stm32_alternate_function_mux<Miso, miso_af::value, 0>::connect_cas,
                typename stm32_alternate_function_mux<Mosi, mosi_af::value, 0>::connect_cas,
                typename stm32_alternate_function_mux<Sclk, sclk_af::value, 0>::connect_cas>;

            using clock_on = api::optimize<clock_miso_on, clock_mosi_on, clock_sclk_on>;
            using disconnect = api::optimize<api::mode<pin_mode::input, Miso, Mosi, Sclk>>;
        };

        template<class I, class SdaPin, class SclPin>
        class i2c {
        public:
            using sda_af = zoal::metadata::stm32_signal_map<sign, I::address, SdaPin::port::address, SdaPin::offset, signal::sda>;
            using scl_af = zoal::metadata::stm32_signal_map<sign, I::address, SclPin::port::address, SclPin::offset, signal::scl>;

            static_assert(sda_af::value >= 0, "Unsupported SDA pin mapping");
            static_assert(scl_af::value >= 0, "Unsupported SCL pin mapping");

            using clock_sda_on = typename SdaPin::port::clock_on_cas;
            using clock_scl_on = typename SclPin::port::clock_on_cas;

            using clock_on = zoal::gpio::api::optimize<clock_sda_on, clock_scl_on>;
            using connect = api::optimize<
                //
                typename stm32_alternate_function_mux<SdaPin, sda_af::value, 1>::connect_cas,
                typename stm32_alternate_function_mux<SclPin, scl_af::value, 1>::connect_cas>;
            using disconnect = api::optimize<api::mode<pin_mode::input, SdaPin, SclPin>>;
        };

        template<class T, class Pin>
        using adc_channel = typename adc_channel_builder<sign, T, Pin>::result;

        template<class T, class Pin>
        using pwm_channel = typename pwm_channel_builder<sign, T, Pin>::result;
    };
}}}

#endif

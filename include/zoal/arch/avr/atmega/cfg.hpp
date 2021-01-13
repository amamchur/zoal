#ifndef ZOAL_ARCH_AVR_ATMEGA_CFG_HPP
#define ZOAL_ARCH_AVR_ATMEGA_CFG_HPP

#include "../../../ct/type_list.hpp"
#include "../../../gpio/api.hpp"
#include "../../../periph/adc.hpp"
#include "../../../periph/spi.hpp"
#include "../../../periph/timer_mode.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<uint8_t Bits>
    struct usart_data_bits_flags;

    template<::zoal::periph::usart_parity Parity>
    struct usart_parity_flags;

    template<::zoal::periph::usart_stop_bits StopBits>
    struct usart_stop_bit_flags;

    template<class T, ::zoal::periph::timer_mode Mode>
    struct timer_mode;

    template<class T, bool async, uintptr_t ClockDivider>
    struct timer_clock_divider;

    template<class A, zoal::periph::adc_ref Ref>
    struct adc_ref;

    template<class A, uintptr_t ClockDivider>
    struct adc_clock_divider;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    using zoal::ct::type_list;
    using zoal::metadata::adc_clock_divider;
    using zoal::metadata::adc_ref;
    using zoal::metadata::timer_clock_divider;
    using zoal::metadata::timer_mode;
    using zoal::metadata::usart_data_bits_flags;
    using zoal::metadata::usart_parity_flags;
    using zoal::metadata::usart_stop_bit_flags;
    using zoal::periph::bit_order;
    using zoal::periph::spi_cpol_cpha;
    using zoal::periph::spi_mode;
    using zoal::periph::usart_parity;
    using zoal::periph::usart_stop_bits;

    template<uint8_t Bits, usart_parity Parity, usart_stop_bits StopBits>
    struct usart_mode_cfg {
        using db_flags = usart_data_bits_flags<Bits>;
        using pt_flags = usart_parity_flags<Parity>;
        using sb_flags = usart_stop_bit_flags<StopBits>;
        static constexpr auto UCSRxC_value = db_flags::flags | pt_flags::flags | sb_flags::flags;
    };

    template<class S, bit_order Order>
    struct spi_bit_order_cfg : type_list<typename S::SPCRx::template cas<0 << 5, 0 << 5>> {};

    template<class S>
    struct spi_bit_order_cfg<S, bit_order::msbf> : type_list<typename S::SPCRx::template cas<1 << 5, 0 << 5>> {};

    template<class S>
    struct spi_bit_order_cfg<S, bit_order::lsbf> : type_list<typename S::SPCRx::template cas<1 << 5, 1 << 5>> {};

    template<class S, uintptr_t D>
    struct spi_clock_divider_cfg : type_list<typename S::SPCRx::template cas<0x00, 0x00>, typename S::SPSRx::template cas<0x00, 0x00>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 2> : type_list<typename S::SPCRx::template cas<0x03, 0x00>, typename S::SPSRx::template cas<0x01, 0x01>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 4> : type_list<typename S::SPCRx::template cas<0x03, 0x00>, typename S::SPSRx::template cas<0x01, 0x00>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 8> : type_list<typename S::SPCRx::template cas<0x03, 0x01>, typename S::SPSRx::template cas<0x01, 0x01>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 16> : type_list<typename S::SPCRx::template cas<0x03, 0x01>, typename S::SPSRx::template cas<0x01, 0x00>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 32> : type_list<typename S::SPCRx::template cas<0x03, 0x02>, typename S::SPSRx::template cas<0x01, 0x01>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 64> : type_list<typename S::SPCRx::template cas<0x03, 0x02>, typename S::SPSRx::template cas<0x01, 0x00>> {};

    template<class S>
    struct spi_clock_divider_cfg<S, 128> : type_list<typename S::SPCRx::template cas<0x03, 0x03>, typename S::SPSRx::template cas<0x01, 0x00>> {};

    template<class S, spi_cpol_cpha PolPha>
    struct spi_cpol_cpha_cfg : type_list<typename S::SPCRx::template cas<0x00, 0x00>> {};

    template<class S>
    struct spi_cpol_cpha_cfg<S, spi_cpol_cpha::mode_0> : type_list<typename S::SPCRx::template cas<0x03 << 2, 0x00 << 2>> {};

    template<class S>
    struct spi_cpol_cpha_cfg<S, spi_cpol_cpha::mode_1> : type_list<typename S::SPCRx::template cas<0x03 << 2, 0x01 << 2>> {};

    template<class S>
    struct spi_cpol_cpha_cfg<S, spi_cpol_cpha::mode_2> : type_list<typename S::SPCRx::template cas<0x03 << 2, 0x02 << 2>> {};

    template<class S>
    struct spi_cpol_cpha_cfg<S, spi_cpol_cpha::mode_3> : type_list<typename S::SPCRx::template cas<0x03 << 2, 0x03 << 2>> {};

    template<class S, spi_mode Mode>
    struct spi_mode_cfg : type_list<typename S::SPCRx::template cas<0x00 << 4, 0x00 << 4>> {};

    template<class S>
    struct spi_mode_cfg<S, spi_mode::master> : type_list<typename S::SPCRx::template cas<0x01 << 4, 0x01 << 4>> {};

    template<class S>
    struct spi_mode_cfg<S, spi_mode::slave> : type_list<typename S::SPCRx::template cas<0x01 << 4, 0x03 << 4>> {};

    template<uint32_t Frequency>
    class cfg {
    public:
        static constexpr auto frequency = Frequency;

        template<class U, class Cfg>
        class usart {
        public:
            static constexpr uint8_t UCSRxA_bit_U2X = 1 << 1;
            static constexpr auto value_x2 = (Cfg::clock_frequency / (8 * Cfg::baud_rate)) - 1;
            static constexpr auto value_x1 = (Cfg::clock_frequency / (16 * Cfg::baud_rate)) - 1;
            static constexpr auto value = value_x2 > 4095 ? value_x1 : value_x2;
            static constexpr uint8_t UCSRxA_value = static_cast<const uint8_t>(value_x2 > 4095 ? 0u : UCSRxA_bit_U2X);
            static constexpr uint8_t UBRRxL_value = static_cast<uint8_t>(value & 0xFFu);
            static constexpr uint8_t UBRRxH_value = static_cast<uint8_t>(value >> 0x8u);

            using apply = typename zoal::gpio::api::optimize<
                type_list<typename U::UBRRxL::template cas<0xFF, UBRRxL_value>,
                          typename U::UBRRxH::template cas<0xFF, UBRRxH_value>,
                          typename U::UCSRxA::template cas<0xFF, UCSRxA_value>,
                          typename U::UCSRxC::template cas<0xFF, usart_mode_cfg<Cfg::word_length_bits, Cfg::parity, Cfg::stop_bits>::UCSRxC_value>>>;
        };

        template<class T, zoal::periph::timer_mode Mode, uintptr_t ClockDivider, uintptr_t Prescale, uintptr_t Period>
        class timer {
        public:
            static_assert(Prescale == 1, "Unsupported prescale");

            static constexpr auto async = T::async;
            using timer_mode_cfg = timer_mode<T, Mode>;
            using clock_divider_cfg = timer_clock_divider<T, async, ClockDivider>;
            using apply = typename zoal::gpio::api::optimize<timer_mode_cfg, clock_divider_cfg>;
        };

        template<class A, zoal::periph::adc_ref Ref = zoal::periph::adc_ref::vcc, uintptr_t ClockDivider = 128>
        class adc {
        public:
            using apply = typename zoal::gpio::api::optimize<
                //
                adc_clock_divider<A, ClockDivider>,
                adc_ref<A, Ref>>;
        };

        template<class S,
                 uintptr_t ClockDivider = 2,
                 bit_order Order = bit_order::msbf,
                 spi_cpol_cpha PolPha = spi_cpol_cpha::mode_0,
                 spi_mode Mode = spi_mode::master>
        class spi {
        public:
            using clock_divider = spi_clock_divider_cfg<S, ClockDivider>;
            using order = spi_bit_order_cfg<S, Order>;
            using cpol_cpha = spi_cpol_cpha_cfg<S, PolPha>;
            using mode = spi_mode_cfg<S, Mode>;
            using apply = typename zoal::gpio::api::optimize<clock_divider, order, cpol_cpha, mode>::result;
        };

        template<class I, uint32_t Freq = 400000>
        class i2c {
        public:
            static constexpr uint8_t TWBRx_value = static_cast<uint8_t>((((double)Frequency / (double)Freq) - 16.0) / 2.0);
            using apply = typename zoal::gpio::api::optimize<
                type_list<typename I::TWSRx::template cas<0xFF, 0>, typename I::TWBRx::template cas<0xFF, TWBRx_value>>>;
        };
    };
}}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_CFG_HPP
#define ZOAL_ARCH_AVR_ATMEGA_CFG_HPP

#include "../../../ct/type_list.hpp"
#include "../../../gpio/api.hpp"
#include "../../../mem/accessor.hpp"
#include "../../../mem/clear_and_set.hpp"
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

    template<zoal::periph::adc_ref Ref>
    struct adc_ref;

    template<uintptr_t ClockDivider>
    struct adc_clock_divider;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    using zoal::ct::type_list;
    using zoal::mem::clear_and_set;
    using zoal::mem::merge_clear_and_set;
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

        template<class U,
                 uint32_t BaudRate,
                 uint8_t Bits = 8,
                 usart_parity Parity = usart_parity::none,
                 usart_stop_bits StopBits = usart_stop_bits::stop_bits_1,
                 uint32_t Freq = frequency>
        class usart {
        public:
            static constexpr uint8_t UCSRxA_bit_U2X = 1 << 1;
            static constexpr auto value_x2 = (Freq / (8 * BaudRate)) - 1;
            static constexpr auto value_x1 = (Freq / (16 * BaudRate)) - 1;
            static constexpr auto value = value_x2 > 4095 ? value_x1 : value_x2;
            static constexpr uint8_t UCSRxA_value = static_cast<const uint8_t>(value_x2 > 4095 ? 0u : UCSRxA_bit_U2X);
            static constexpr uint8_t UBRRxL_value = static_cast<uint8_t>(value & 0xFFu);
            static constexpr uint8_t UBRRxH_value = static_cast<uint8_t>(value >> 0x8u);

            static void apply() {
                U::disable();

                U::UBRRxL::ref() = UBRRxL_value;
                U::UBRRxH::ref() = UBRRxH_value;
                U::UCSRxA::ref() = UCSRxA_value;
                U::UCSRxC::ref() = usart_mode_cfg<Bits, Parity, StopBits>::UCSRxC_value;
            }
        };

        template<class T, zoal::periph::timer_mode Mode, uintptr_t ClockDivider, uintptr_t Prescale, uintptr_t Period>
        class timer {
        public:
            static_assert(Prescale == 1, "Unsupported prescale");

            static constexpr auto async = T::async;
            using timer_mode_cfg = timer_mode<T, Mode>;
            using clock_divider_cfg = timer_clock_divider<T, async, ClockDivider>;
            using list = typename zoal::gpio::api_new::apply<timer_mode_cfg, clock_divider_cfg>::result;

            static void apply() {
                T::disable();

                zoal::mem::apply_cas_list<list>();
            }
        };

        template<class A, zoal::periph::adc_ref Ref = zoal::periph::adc_ref::vcc, uintptr_t ClockDivider = 128>
        class adc {
        public:
            using ADCSRAx_cfg = typename adc_clock_divider<ClockDivider>::ADCSRAx;
            using ADMUXx_cfg = typename adc_ref<Ref>::ADMUXx;

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, A::address, Offset>;

            static void apply() {
                A::disable();

                ADMUXx_cfg::apply(accessor<A::ADMUXx>::ref());
                ADCSRAx_cfg::apply(accessor<A::ADCSRAx>::ref());
            }
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

            using list = typename zoal::gpio::api_new::apply<clock_divider, order, cpol_cpha, mode>::result;

            static void apply() {
                S::disable();

                zoal::mem::apply_cas_list<list>();
            }
        };

        template<class I, uint32_t Freq = 400000>
        class i2c {
        public:
            static constexpr uint8_t TWBRx_value = static_cast<uint8_t>((((double)Frequency / (double)Freq) - 16.0) / 2.0);

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, I::address, Offset>;

            static void apply() {
                I::disable();

                I::TWSRx::ref() = 0;
                I::TWBRx::ref() = TWBRx_value;

                //                *accessor<TWSRx>::ref() &= ~(1 << TWPS0x | 1 << TWPS1x);
                //                *accessor<TWBRx>::ref() = ((Config::freq / Config::i2c_freq) - 16) / 2;
            }
        };
    };
}}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_CFG_HPP
#define ZOAL_ARCH_AVR_ATMEGA_CFG_HPP

#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/segment.hpp"
#include "zoal/periph/adc.hpp"
#include "../../../periph/timer_mode.hpp"
#include "zoal/periph/usart.hpp"
#include "../../../utils/helpers.hpp"
#include "zoal/periph/spi.hpp"

namespace zoal { namespace metadata {
    template<uint8_t Bits>
    struct usart_data_bits_flags;

    template<::zoal::periph::usart_parity Parity>
    struct usart_parity_flags;

    template<::zoal::periph::usart_stop_bits StopBits>
    struct usart_stop_bit_flags;

    template<::zoal::periph::timer_mode Mode>
    struct timer_mode;

    template<bool async, uintptr_t ClockDivider>
    struct timer_clock_divider;

    template<zoal::periph::adc_ref Ref>
    struct adc_ref;

    template<uintptr_t ClockDivider>
    struct adc_clock_divider;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    using zoal::mem::clear_and_set;
    using zoal::mem::merge_clear_and_set;
    using zoal::mem::segment;
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

    template<bit_order Order>
    struct spi_bit_order_cfg {
        using SPCRx = clear_and_set<0, 0>;
    };

    template<>
    struct spi_bit_order_cfg<bit_order::msbf> {
        using SPCRx = clear_and_set<1, 0, 5>;
    };

    template<>
    struct spi_bit_order_cfg<bit_order::lsbf> {
        using SPCRx = clear_and_set<1, 1, 5>;
    };

    template<uintptr_t D>
    struct spi_clock_divider_cfg {
        using SPCRx = clear_and_set<0, 0>;
        using SPSRx = clear_and_set<0, 0>;
    };

    template<>
    struct spi_clock_divider_cfg<2> {
        using SPCRx = clear_and_set<0x03, 0x00>;
        using SPSRx = clear_and_set<0x01, 0x01>;
    };

    template<>
    struct spi_clock_divider_cfg<4> {
        using SPCRx = clear_and_set<0x03, 0x00>;
        using SPSRx = clear_and_set<0x01, 0x00>;
    };

    template<>
    struct spi_clock_divider_cfg<8> {
        using SPCRx = clear_and_set<0x03, 0x01>;
        using SPSRx = clear_and_set<0x01, 0x01>;
    };

    template<>
    struct spi_clock_divider_cfg<16> {
        using SPCRx = clear_and_set<0x03, 0x01>;
        using SPSRx = clear_and_set<0x01, 0x00>;
    };

    template<>
    struct spi_clock_divider_cfg<32> {
        using SPCRx = clear_and_set<0x03, 0x02>;
        using SPSRx = clear_and_set<0x01, 0x01>;
    };

    template<>
    struct spi_clock_divider_cfg<64> {
        using SPCRx = clear_and_set<0x03, 0x02>;
        using SPSRx = clear_and_set<0x01, 0x00>;
    };

    template<>
    struct spi_clock_divider_cfg<128> {
        using SPCRx = clear_and_set<0x03, 0x03>;
        using SPSRx = clear_and_set<0x01, 0x00>;
    };

    template<spi_cpol_cpha PolPha>
    struct spi_cpol_cpha_cfg {
        using SPCRx = clear_and_set<0x00, 0x00>;
    };

    template<>
    struct spi_cpol_cpha_cfg<spi_cpol_cpha::mode_0> {
        using SPCRx = clear_and_set<0x03, 0x00, 2>;
    };

    template<>
    struct spi_cpol_cpha_cfg<spi_cpol_cpha::mode_1> {
        using SPCRx = clear_and_set<0x03, 0x01, 2>;
    };

    template<>
    struct spi_cpol_cpha_cfg<spi_cpol_cpha::mode_2> {
        using SPCRx = clear_and_set<0x03, 0x02, 2>;
    };

    template<>
    struct spi_cpol_cpha_cfg<spi_cpol_cpha::mode_3> {
        using SPCRx = clear_and_set<0x03, 0x03, 2>;
    };

    template<spi_mode Mode>
    struct spi_mode_cfg {
        using SPCRx = clear_and_set<0x00, 0x00>;
    };

    template<>
    struct spi_mode_cfg<spi_mode::master> {
        using SPCRx = clear_and_set<0x01, 0x01, 4>;
    };

    template<>
    struct spi_mode_cfg<spi_mode::slave> {
        using SPCRx = clear_and_set<0x01, 0x00, 4>;
    };

    template<class Api, uint32_t Frequency>
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

                segment<uint8_t, U::address> mem;
                mem[U::UBRRxL] = UBRRxL_value;
                mem[U::UBRRxH] = UBRRxH_value;
                mem[U::UCSRxA] = UCSRxA_value;
                mem[U::UCSRxC] = usart_mode_cfg<Bits, Parity, StopBits>::UCSRxC_value;
                mem.happyInspection();
            }
        };

        template<class T, zoal::periph::timer_mode Mode, uintptr_t ClockDivider, uintptr_t Prescale, uintptr_t Period>
        class timer {
        public:
            static_assert(Prescale == 1, "Unsupported prescale");

            static constexpr auto async = T::async;
            using timer_mode_cfg = timer_mode<Mode>;
            using clock_divider_cfg = timer_clock_divider<async, ClockDivider>;
            using TCCRxA_cfg = typename timer_mode<Mode>::TCCRxA;
            using TCCRxB_cfg = merge_clear_and_set<typename timer_mode_cfg::TCCRxB, typename clock_divider_cfg::TCCRxB>;

            static void apply() {
                T::disable();

                segment<uint8_t, T::address> mem;
                TCCRxA_cfg::apply(mem[T::TCCRxA]);
                TCCRxB_cfg::apply(mem[T::TCCRxB]);
            }
        };

        template<class A, zoal::periph::adc_ref Ref = zoal::periph::adc_ref::external, uintptr_t ClockDivider = 128>
        class adc {
        public:
            using ADCSRAx_cfg = typename adc_clock_divider<ClockDivider>::ADCSRAx;
            using ADMUXx_cfg = typename adc_ref<Ref>::ADMUXx;

            static void apply() {
                A::disable();

                segment<uint8_t, A::address> mem;
                ADMUXx_cfg::apply(mem[A::ADMUXx]);
                ADCSRAx_cfg::apply(mem[A::ADCSRAx]);
            }
        };

        template<class S,
                 uintptr_t ClockDivider = 2,
                 bit_order Order = bit_order::msbf,
                 spi_cpol_cpha PolPha = spi_cpol_cpha::mode_0,
                 spi_mode Mode = spi_mode::master>
        class spi {
        public:
            using clock_divider = spi_clock_divider_cfg<ClockDivider>;
            using order = spi_bit_order_cfg<Order>;
            using cpol_cpha = spi_cpol_cpha_cfg<PolPha>;
            using mode = spi_mode_cfg<Mode>;

            static_assert(clock_divider::SPCRx::clear_mask != 0, "Unsupported SPI clock divider");
            static_assert(clock_divider::SPSRx::clear_mask != 0, "Unsupported SPI clock divider");
            static_assert(cpol_cpha::SPCRx::clear_mask != 0, "Unsupported SPI polarity and/or phase");
            static_assert(mode::SPCRx::clear_mask != 0, "Unsupported SPI mode");
            static_assert(order::SPCRx::clear_mask != 0, "Unsupported SPI bit order");

            using SPCRx = merge_clear_and_set<typename order::SPCRx,
                                              typename clock_divider::SPCRx,
                                              typename cpol_cpha::SPCRx,
                                              typename mode::SPCRx>;
            using SPSRx = typename clock_divider::SPSRx;

            static void apply() {
                S::disable();

                segment<uint8_t, S::address> mem;
                SPCRx::apply(mem[S::SPCRx]);
                SPSRx::apply(mem[S::SPSRx]);
            }
        };

        template<class I, uint32_t Freq = 400000>
        class i2c {
        public:
            static constexpr uint8_t TWBRx_value = static_cast<uint8_t>((((double)Frequency / (double)Freq) - 16.0) / 2.0);
            static void apply() {
                I::disable();

                segment<uint8_t, I::address> mem;
                mem[I::TWBRx] = TWBRx_value;

//                mem[TWSRx] &= ~(1 << TWPS0x | 1 << TWPS1x);
//                mem[TWBRx] = ((Config::freq / Config::i2c_freq) - 16) / 2;
            }
        };
    };
}}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_METADATA_HPP
#define ZOAL_ARCH_AVR_ATMEGA_METADATA_HPP

#include "../../../mem/clear_and_set.hpp"
#include "../../../mem/segment.hpp"
#include "../../../periph/adc.hpp"
#include "../../../periph/timer_mode.hpp"
#include "../../../periph/usart.hpp"
#include "../../../utils/helpers.hpp"
#include "../metadata.hpp"

namespace zoal { namespace metadata {
    template<uint8_t Bits>
    struct usart_data_bits_flags {};

    template<>
    struct usart_data_bits_flags<5> {
        static constexpr uint8_t flags = 0 << 1;
    };

    template<>
    struct usart_data_bits_flags<6> {
        static constexpr uint8_t flags = 1 << 1;
    };

    template<>
    struct usart_data_bits_flags<7> {
        static constexpr uint8_t flags = 2 << 1;
    };

    template<>
    struct usart_data_bits_flags<8> {
        static constexpr uint8_t flags = 3 << 1;
    };

    template<::zoal::periph::usart_parity Parity>
    struct usart_parity_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct usart_parity_flags<::zoal::periph::usart_parity::even> {
        static constexpr uint8_t flags = 2 << 4;
    };

    template<>
    struct usart_parity_flags<::zoal::periph::usart_parity::odd> {
        static constexpr uint8_t flags = 3 << 4;
    };

    template<::zoal::periph::usart_stop_bits StopBits>
    struct usart_stop_bit_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct usart_stop_bit_flags<::zoal::periph::usart_stop_bits::stop_bits_2> {
        static constexpr uint8_t flags = 1 << 3;
    };

    enum : uint8_t {
        WGMx0 = 0,
        WGMx1 = 1,
        WGMx2 = 3,
        WGMx3 = 4,
        mode_clear_TCCRxA = 1u << WGMx0 | 1u << WGMx1,
        mode_clear_TCCRxB = 1u << WGMx2 | 1u << WGMx3
    };

    template<>
    struct timer_mode<::zoal::periph::timer_mode::up> {
        using TCCRxA = zoal::mem::clear_and_set<mode_clear_TCCRxA, 1u << WGMx0 | 1u << WGMx1>;
        using TCCRxB = zoal::mem::clear_and_set<mode_clear_TCCRxB, 0x0>;
    };

    template<>
    struct timer_mode<::zoal::periph::timer_mode::up_down> {
        using TCCRxA = zoal::mem::clear_and_set<mode_clear_TCCRxA, 1u << WGMx0>;
        using TCCRxB = zoal::mem::clear_and_set<mode_clear_TCCRxB, 0x0>;
    };

    template<uintptr_t Set>
    struct tcd_cas {
        using TCCRxB = zoal::mem::clear_and_set<0x7, Set>;
    };

    template<>
    struct timer_clock_divider<false, 0> : tcd_cas<0x0> {};

    template<>
    struct timer_clock_divider<false, 1> : tcd_cas<0x1> {};

    template<>
    struct timer_clock_divider<false, 8> : tcd_cas<0x2> {};

    template<>
    struct timer_clock_divider<false, 64> : tcd_cas<0x3> {};

    template<>
    struct timer_clock_divider<false, 256> : tcd_cas<0x4> {};

    template<>
    struct timer_clock_divider<false, 1024> : tcd_cas<0x5> {};

    template<>
    struct timer_clock_divider<true, 0> : tcd_cas<0x0> {};

    template<>
    struct timer_clock_divider<true, 1> : tcd_cas<0x1> {};

    template<>
    struct timer_clock_divider<true, 8> : tcd_cas<0x2> {};

    template<>
    struct timer_clock_divider<true, 32> : tcd_cas<0x3> {};

    template<>
    struct timer_clock_divider<true, 64> : tcd_cas<0x4> {};

    template<>
    struct timer_clock_divider<true, 128> : tcd_cas<0x5> {};

    template<>
    struct timer_clock_divider<true, 256> : tcd_cas<0x6> {};

    template<>
    struct timer_clock_divider<true, 1024> : tcd_cas<0x7> {};

    struct timer_clock_dividers : zoal::ct::value_list<uintptr_t, 0, 1, 8, 32, 64, 128, 256, 1024> {};

    template<>
    struct adc_ref<zoal::periph::adc_ref::internal> {
        using ADMUXx = zoal::mem::clear_and_set<0xC0, 0xC0>;
    };

    template<>
    struct adc_ref<zoal::periph::adc_ref::external> {
        using ADMUXx = zoal::mem::clear_and_set<0xC0, 0x40>;
    };

    template<uintptr_t ClockDivider>
    struct adc_clock_divider {};

    template<>
    struct adc_clock_divider<2> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x01>;
    };

    template<>
    struct adc_clock_divider<4> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x02>;
    };

    template<>
    struct adc_clock_divider<8> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x03>;
    };

    template<>
    struct adc_clock_divider<16> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x04>;
    };

    template<>
    struct adc_clock_divider<32> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x05>;
    };

    template<>
    struct adc_clock_divider<64> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x06>;
    };

    template<>
    struct adc_clock_divider<128> {
        using ADCSRAx = zoal::mem::clear_and_set<0x07, 0x07>;
    };
}}

#endif

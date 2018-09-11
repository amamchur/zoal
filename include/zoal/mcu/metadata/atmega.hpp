#ifndef ZOAL_MCU_METADATA_HPP
#define ZOAL_MCU_METADATA_HPP

#include "../../periph/adc_config.hpp"
#include "../../periph/timer_mode.hpp"
#include "../../periph/usart_config.hpp"
#include "../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<::zoal::periph::usart_data_bits Bits>
    struct usart_data_bits_flags {};

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_5> {
        static constexpr uint8_t flags = 0 << 1;
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_6> {
        static constexpr uint8_t flags = 1 << 1;
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_7> {
        static constexpr uint8_t flags = 2 << 1;
    };

    template<>
    struct usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_8> {
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

    template<int Rx, int Tx, int Ck>
    struct base_usart_mapping {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct usart_mapping : public base_usart_mapping<-1, -1, -1> {
    };

    enum : uint8_t
    {
        WGMx0 = 0,
        WGMx1 = 1,
        WGMx2 = 3,
        WGMx3 = 4,
        mode_clear_TCCRxA = 1u << WGMx0 | 1u << WGMx1,
        mode_clear_TCCRxB = 1u << WGMx2 | 1u << WGMx3
    };

    template<::zoal::periph::timer_mode Mode>
    struct timer_mode {};

    template<>
    struct timer_mode<::zoal::periph::timer_mode::up> {
        using TCCRxA = zoal::utils::clear_and_set<mode_clear_TCCRxA, 1u << WGMx0 | 1u << WGMx1>;
        using TCCRxB = zoal::utils::clear_and_set<mode_clear_TCCRxB, 0x0>;
    };

    template<>
    struct timer_mode<::zoal::periph::timer_mode::up_down> {
        using TCCRxA = zoal::utils::clear_and_set<mode_clear_TCCRxA, 1u << WGMx0>;
        using TCCRxB = zoal::utils::clear_and_set<mode_clear_TCCRxB, 0x0>;
    };

    template<bool async, uintptr_t ClockDivider>
    struct timer_clock_divider {
        static_assert(ClockDivider <= 0, "Unsupported clock divider");
    };

    template<uintptr_t Set>
    struct tcd_cas {
        using TCCRxB = zoal::utils::clear_and_set<0x7, Set>;
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

    struct timer_clock_dividers : zoal::utils::value_list<uintptr_t, 0, 1, 8, 32, 64, 128, 256, 1024> {};

    template<zoal::periph::adc_ref Ref>
    struct adc_ref {};

    template<>
    struct adc_ref<zoal::periph::adc_ref::internal> {
        using ADMUXx = zoal::utils::clear_and_set<0xC0, 0xC0>;
    };

    template<>
    struct adc_ref<zoal::periph::adc_ref::external> {
        using ADMUXx = zoal::utils::clear_and_set<0xC0, 0x40>;
    };

    template<uintptr_t ClockDivider>
    struct adc_clock_divider {};

    template<>
    struct adc_clock_divider<2> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x01>;
    };

    template<>
    struct adc_clock_divider<4> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x02>;
    };

    template<>
    struct adc_clock_divider<8> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x03>;
    };

    template<>
    struct adc_clock_divider<16> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x04>;
    };

    template<>
    struct adc_clock_divider<32> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x05>;
    };

    template<>
    struct adc_clock_divider<64> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x06>;
    };

    template<>
    struct adc_clock_divider<128> {
        using ADCSRAx = zoal::utils::clear_and_set<0x07, 0x07>;
    };

    template<uintptr_t AdcAddress, uintptr_t PortAddress, uint8_t PinOffset>
    struct pin_to_adc_channel {
        static constexpr int channel = -1;
    };

    template<class Adc, class Pin>
    struct adc_mapping : pin_to_adc_channel<Adc::address, Pin::port::address, Pin::offset> {
    };
}}

#endif

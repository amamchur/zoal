#ifndef ZOAL_ARCH_AVR_ATTINY_METADATA_HPP
#define ZOAL_ARCH_AVR_ATTINY_METADATA_HPP

#include "../metadata.hpp"

namespace zoal { namespace metadata {
    enum : uint8_t { WGMx0 = 0, WGMx1 = 1, WGMx2 = 3, WGMx3 = 4, mode_clear_TCCRxA = 1u << WGMx0 | 1u << WGMx1, mode_clear_TCCRxB = 1u << WGMx2 | 1u << WGMx3 };

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

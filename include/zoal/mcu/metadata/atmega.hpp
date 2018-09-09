#ifndef ZOAL_MCU_METADATA_ATMEGA_HPP
#define ZOAL_MCU_METADATA_ATMEGA_HPP

#include "../../periph/timer_mode.hpp"
#include "../../periph/usart_config.hpp"
#include "../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<::zoal::periph::usart_data_bits Bits>
    struct atmega_usart_data_bits_flags {};

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_5> {
        static constexpr uint8_t flags = 0 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_6> {
        static constexpr uint8_t flags = 1 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_7> {
        static constexpr uint8_t flags = 2 << 1;
    };

    template<>
    struct atmega_usart_data_bits_flags<::zoal::periph::usart_data_bits::data_bits_8> {
        static constexpr uint8_t flags = 3 << 1;
    };

    template<::zoal::periph::usart_parity Parity>
    struct atmega_usart_parity_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct atmega_usart_parity_flags<::zoal::periph::usart_parity::even> {
        static constexpr uint8_t flags = 2 << 4;
    };

    template<>
    struct atmega_usart_parity_flags<::zoal::periph::usart_parity::odd> {
        static constexpr uint8_t flags = 3 << 4;
    };

    template<::zoal::periph::usart_stop_bits StopBits>
    struct atmega_usart_stop_bit_flags {
        static constexpr uint8_t flags = 0;
    };

    template<>
    struct atmega_usart_stop_bit_flags<::zoal::periph::usart_stop_bits::stop_bits_2> {
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

    template<::zoal::periph::timer_mode Mode>
    struct atmega_timer_mode {};

    template<>
    struct atmega_timer_mode<::zoal::periph::timer_mode::up> {
        using TCCRxA = zoal::utils::clear_and_set<mode_clear_TCCRxA, 1u << WGMx0 | 1u << WGMx1>;
        using TCCRxB = zoal::utils::clear_and_set<mode_clear_TCCRxB, 0x0>;
    };

    template<>
    struct atmega_timer_mode<::zoal::periph::timer_mode::up_down> {
        using TCCRxA = zoal::utils::clear_and_set<mode_clear_TCCRxA, 1u << WGMx0>;
        using TCCRxB = zoal::utils::clear_and_set<mode_clear_TCCRxB, 0x0>;
    };

    template<bool async, uintptr_t ClockDivider>
    struct atmega_timer_clock_divider {
        static_assert(ClockDivider <= 0, "Unsupported clock divider");
    };

    template<uintptr_t Set>
    struct atmega_tcd_cas {
        using TCCRxB = zoal::utils::clear_and_set<0x7, Set>;
    };

    template<>
    struct atmega_timer_clock_divider<false, 0> : atmega_tcd_cas<0x0> {};

    template<>
    struct atmega_timer_clock_divider<false, 1> : atmega_tcd_cas<0x1> {};

    template<>
    struct atmega_timer_clock_divider<false, 8> : atmega_tcd_cas<0x2> {};

    template<>
    struct atmega_timer_clock_divider<false, 64> : atmega_tcd_cas<0x3> {};

    template<>
    struct atmega_timer_clock_divider<false, 256> : atmega_tcd_cas<0x4> {};

    template<>
    struct atmega_timer_clock_divider<false, 1024> : atmega_tcd_cas<0x5> {};

    template<>
    struct atmega_timer_clock_divider<true, 0> : atmega_tcd_cas<0x0> {};

    template<>
    struct atmega_timer_clock_divider<true, 1> : atmega_tcd_cas<0x1> {};

    template<>
    struct atmega_timer_clock_divider<true, 8> : atmega_tcd_cas<0x2> {};

    template<>
    struct atmega_timer_clock_divider<true, 32> : atmega_tcd_cas<0x3> {};

    template<>
    struct atmega_timer_clock_divider<true, 64> : atmega_tcd_cas<0x4> {};

    template<>
    struct atmega_timer_clock_divider<true, 128> : atmega_tcd_cas<0x5> {};

    template<>
    struct atmega_timer_clock_divider<true, 256> : atmega_tcd_cas<0x6> {};

    template<>
    struct atmega_timer_clock_divider<true, 1024> : atmega_tcd_cas<0x7> {};
}}

#endif

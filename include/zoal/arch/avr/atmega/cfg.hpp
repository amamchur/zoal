#ifndef ZOAL_ARCH_AVR_CFG_HPP
#define ZOAL_ARCH_AVR_CFG_HPP

#include "../../../periph/adc_config.hpp"
#include "../../../periph/timer_mode.hpp"
#include "../../../periph/usart_config.hpp"
#include "../../../utils/helpers.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace metadata {
    template<::zoal::periph::usart_data_bits Bits>
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
    using zoal::metadata::adc_clock_divider;
    using zoal::metadata::adc_ref;
    using zoal::metadata::timer_clock_divider;
    using zoal::metadata::timer_mode;
    using zoal::metadata::usart_data_bits_flags;
    using zoal::metadata::usart_parity_flags;
    using zoal::metadata::usart_stop_bit_flags;
    using zoal::periph::usart_data_bits;
    using zoal::periph::usart_parity;
    using zoal::periph::usart_stop_bits;
    using zoal::utils::clear_and_set;
    using zoal::utils::memory_segment;
    using zoal::utils::merge_clear_and_set;

    template<usart_data_bits Bits, usart_parity Parity, usart_stop_bits StopBits>
    struct usart_mode {
        using db_flags = usart_data_bits_flags<Bits>;
        using pt_flags = usart_parity_flags<Parity>;
        using sb_flags = usart_stop_bit_flags<StopBits>;
        static constexpr auto UCSRxC_value = db_flags::flags | pt_flags::flags | sb_flags::flags;
    };

    template<class Api, uint32_t Frequency>
    class cfg {
    public:
        static constexpr auto frequency = Frequency;

        template<class U,
                 uint32_t BaudRate,
                 usart_data_bits Bits = usart_data_bits::data_bits_8,
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

                memory_segment<uint8_t, U::address> mem;
                mem[U::UBRRxL] = UBRRxL_value;
                mem[U::UBRRxH] = UBRRxH_value;
                mem[U::UCSRxA] = UCSRxA_value;
                mem[U::UCSRxC] = usart_mode<Bits, Parity, StopBits>::UCSRxC_value;
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

                memory_segment<uint8_t, T::address> mem;
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

                memory_segment<uint8_t, A::address> mem;
                ADMUXx_cfg::apply(mem[A::ADMUXx]);
                ADCSRAx_cfg::apply(mem[A::ADCSRAx]);
            }
        };
    };
}}}}

#endif

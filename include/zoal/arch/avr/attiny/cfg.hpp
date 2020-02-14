#ifndef ZOAL_ARCH_AVR_ATTINY_CFG_HPP
#define ZOAL_ARCH_AVR_ATTINY_CFG_HPP

#include "../../../mem/clear_and_set.hpp"
#include "../../../periph/adc.hpp"
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

    template<::zoal::periph::timer_mode Mode>
    struct timer_mode;

    template<bool async, uintptr_t ClockDivider>
    struct timer_clock_divider;

    template<zoal::periph::adc_ref Ref>
    struct adc_ref;

    template<uintptr_t ClockDivider>
    struct adc_clock_divider;
}}

namespace zoal { namespace arch { namespace avr { namespace attiny {
    using zoal::mem::clear_and_set;
    using zoal::mem::merge_clear_and_set;
    using zoal::metadata::adc_clock_divider;
    using zoal::metadata::adc_ref;
    using zoal::metadata::timer_clock_divider;
    using zoal::metadata::timer_mode;
    using zoal::metadata::usart_data_bits_flags;
    using zoal::metadata::usart_parity_flags;
    using zoal::metadata::usart_stop_bit_flags;
    using zoal::periph::usart_parity;
    using zoal::periph::usart_stop_bits;

    template<class Api, uint32_t Frequency>
    class cfg {
    public:
        static constexpr auto frequency = Frequency;

        template<class T, zoal::periph::timer_mode Mode, uintptr_t ClockDivider, uintptr_t Prescale = 1, uintptr_t Period = 0xFF>
        class timer {
        public:
            static_assert(Prescale == 1, "Unsupported prescale");
            static constexpr auto async = T::async;
            using timer_mode_cfg = timer_mode<Mode>;
            using clock_divider_cfg = timer_clock_divider<async, ClockDivider>;

            using TCCRxA_cfg = typename timer_mode<Mode>::TCCRxA;
            using TCCRxB_cfg = typename merge_clear_and_set<typename timer_mode_cfg::TCCRxB, typename clock_divider_cfg::TCCRxB>::result;

            template<uintptr_t Offset>
            using accessor = zoal::mem::accessor<uint8_t, T::address, Offset>;

            static void apply() {
                T::disable();

                TCCRxA_cfg::apply(accessor<T::TCCRxA>::ref());
                TCCRxB_cfg::apply(accessor<T::TCCRxB>::ref());
            }
        };

        template<class A, zoal::periph::adc_ref Ref = zoal::periph::adc_ref::vcc, uintptr_t ClockDivider = 128>
        class adc {
        public:
            using ADCSRAx_cfg = typename adc_clock_divider<ClockDivider>::ADCSRAx;
            using ADMUXx_cfg = typename adc_ref<Ref>::ADMUXx;

            static void apply() {
                A::disable();
            }
        };
    };
}}}}

#endif

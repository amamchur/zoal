#ifndef ZOAL_ARCH_AVR_ATTINY_CFG_HPP
#define ZOAL_ARCH_AVR_ATTINY_CFG_HPP

#include "../../../gpio/api.hpp"
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

    template<class T, ::zoal::periph::timer_mode Mode>
    struct timer_mode;

    template<class T, bool async, uintptr_t ClockDivider>
    struct timer_clock_divider;

    template<class A, zoal::periph::adc_ref Ref>
    struct adc_ref;

    template<class A, uintptr_t ClockDivider>
    struct adc_clock_divider;
}}

namespace zoal { namespace arch { namespace avr { namespace attiny {
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
            static constexpr auto async = T::async;
            using timer_mode_cfg = timer_mode<T, Mode>;
            using clock_divider_cfg = timer_clock_divider<T, async, ClockDivider>;
            using list = typename zoal::gpio::api::optimize<timer_mode_cfg, clock_divider_cfg>;

            static void apply() {
                T::disable();

                zoal::mem::apply_cas_list<list>::apply();
            }
        };

        template<class A, zoal::periph::adc_ref Ref = zoal::periph::adc_ref::vcc, uintptr_t ClockDivider = 128>
        class adc {
        public:
            //            using ADCSRAx_cfg = typename adc_clock_divider<ClockDivider>::ADCSRAx;
            //            using ADMUXx_cfg = typename adc_ref<Ref>::ADMUXx;

            static void apply() {
                A::disable();
            }
        };
    };
}}}}

#endif

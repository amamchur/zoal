#ifndef ZOAL_ARCH_AVR_METADATA_HPP
#define ZOAL_ARCH_AVR_METADATA_HPP

#include "../../ct/constant.hpp"
#include "../../ct/value_list.hpp"
#include "../../periph/adc_config.hpp"
#include "../../periph/timer_mode.hpp"
#include "../../periph/usart_config.hpp"
#include "../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<int Rx, int Tx, int Ck>
    struct base_usart_mapping {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct usart_mapping : public base_usart_mapping<-1, -1, -1> {};

    template<::zoal::periph::timer_mode Mode>
    struct timer_mode {};

    template<bool async, uintptr_t ClockDivider>
    struct timer_clock_divider {
        static_assert(ClockDivider <= 0, "Unsupported clock divider");
    };

    template<zoal::periph::adc_ref Ref>
    struct adc_ref {};

    template<uintptr_t AdcAddress, uintptr_t PortAddress, uint8_t PinOffset>
    struct pin_to_adc_channel {
        static constexpr int channel = -1;
    };

    template<class Adc, class Pin>
    struct adc_mapping : pin_to_adc_channel<Adc::address, Pin::port::address, Pin::offset> {};

    template<uintptr_t TimerAddress, uintptr_t PortAddress, uint8_t PinOffset, uint8_t Channel>
    struct pin_to_pwm_channel : zoal::ct::integral_constant<bool, false> {};

    template<class Timer, class Pin, uint8_t Channel>
    struct pwm_channel_mapping : pin_to_pwm_channel<Timer::address, Pin::port::address, Pin::offset, Channel> {};
}}

#endif

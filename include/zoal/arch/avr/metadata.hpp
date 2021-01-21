#ifndef ZOAL_ARCH_AVR_METADATA_HPP
#define ZOAL_ARCH_AVR_METADATA_HPP

#include "../../ct/constant.hpp"
#include "../../ct/value_list.hpp"
#include "../../periph/adc.hpp"
#include "../../periph/timer_mode.hpp"
#include "../../periph/usart.hpp"
#include "../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<int Rx, int Tx, int Ck>
    struct base_usart_mapping {
        static constexpr int rx = Rx;
        static constexpr int tx = Tx;
        static constexpr int ck = Ck;
    };

    template<int Mosi, int Miso, int Clock, int SlaveSelect>
    struct base_spi_mapping {
        static constexpr int mosi = Mosi;
        static constexpr int miso = Miso;
        static constexpr int clock = Clock;
        static constexpr int slave_select = SlaveSelect;
    };

    template<int SerialDataLine, int SerialClockLine>
    struct base_i2c_mapping {
        static constexpr int sda = SerialDataLine;
        static constexpr int scl = SerialClockLine;
    };

    template<class Sign, uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct usart_mapping : base_usart_mapping<-1, -1, -1> {};

    template<class Sign, uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct spi_mapping : base_spi_mapping<-1, -1, -1, -1> {};

    template<class Sign, uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct i2c_mapping : base_i2c_mapping<-1, -1> {};

    template<class T, ::zoal::periph::timer_mode Mode>
    struct timer_mode {};

    template<class T, bool async, uintptr_t ClockDivider>
    struct timer_clock_divider {
        static_assert(ClockDivider <= 0, "Unsupported clock divider");
    };

    template<class A, zoal::periph::adc_ref Ref>
    struct adc_ref {};

    template<class Sign, uintptr_t AdcAddress, uintptr_t PortAddress, uint8_t PinOffset>
    struct pin_to_adc_channel {
        static constexpr int channel = -1;
    };

    template<class Sign, class Adc, class Pin>
    struct adc_mapping : pin_to_adc_channel<Sign, Adc::address, Pin::port::address, Pin::offset> {};

    template<class Sign, uintptr_t TimerAddress, uintptr_t PortAddress, uint8_t PinOffset, uint8_t Channel>
    struct pin_to_pwm_channel : zoal::ct::integral_constant<bool, false> {};

    template<class Sign, class Timer, class Pin, uint8_t Channel>
    struct pwm_channel_mapping : pin_to_pwm_channel<Sign, Timer::address, Pin::port::address, Pin::offset, Channel> {};
}}

#endif

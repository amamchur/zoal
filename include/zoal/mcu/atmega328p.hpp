/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_ATMEGA328P_HPP
#define ZOAL_MCU_ATMEGA328P_HPP

#include <stdint.h>
#include <zoal/arch/avr/atmega/adc.hpp>
#include <zoal/arch/avr/atmega/cfg.hpp>
#include <zoal/arch/avr/atmega/i2c.hpp>
#include <zoal/arch/avr/atmega/irq.hpp>
#include <zoal/arch/avr/atmega/metadata.hpp>
#include <zoal/arch/avr/atmega/mux.hpp>
#include <zoal/arch/avr/atmega/spi.hpp>
#include <zoal/arch/avr/atmega/timer16.hpp>
#include <zoal/arch/avr/atmega/timer8.hpp>
#include <zoal/arch/avr/atmega/usart.hpp>
#include <zoal/arch/avr/port.hpp>
#include <zoal/arch/enable.hpp>
#include <zoal/arch/power.hpp>
#include <zoal/ct/signature.hpp>
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/gpio/pin.hpp>

namespace zoal { namespace mcu {
    class atmega328p {
    public:
        using self_type = atmega328p;
        using signature = zoal::ct::signature<'A', 'T', 'm', 'e', 'g', 'a', '3', '2', '8', 'P'>;

        template<uintptr_t Address, uint8_t PinMask>
        using port = typename ::zoal::arch::avr::port<Address, PinMask>;

        using port_b = port<0x0023, 0xFF>;
        using port_c = port<0x0026, 0x7F>;
        using port_d = port<0x0029, 0xFF>;

        using timer_00 = ::zoal::arch::avr::atmega::timer8<0x0044, 0, false>;
        using timer_01 = ::zoal::arch::avr::atmega::timer16<0x0080, 1>;
        using timer_02 = ::zoal::arch::avr::atmega::timer8<0x00B0, 2, true>;

        using spi_00 = ::zoal::arch::avr::atmega::spi<0x004C, 0>;

        using i2c_00 = ::zoal::arch::avr::atmega::i2c<0x00B8, 0>;

        using usart_00 = typename ::zoal::arch::avr::atmega::usart<0x00C0>;

        using adc_00 = ::zoal::arch::avr::atmega::adc<0x0078, 0>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using pb_00 = pin<port_b, 0>;
        using pb_01 = pin<port_b, 1>;
        using pb_02 = pin<port_b, 2>;
        using pb_03 = pin<port_b, 3>;
        using pb_04 = pin<port_b, 4>;
        using pb_05 = pin<port_b, 5>;
        using pb_06 = pin<port_b, 6>;
        using pb_07 = pin<port_b, 7>;

        using pc_00 = pin<port_c, 0>;
        using pc_01 = pin<port_c, 1>;
        using pc_02 = pin<port_c, 2>;
        using pc_03 = pin<port_c, 3>;
        using pc_04 = pin<port_c, 4>;
        using pc_05 = pin<port_c, 5>;
        using pc_06 = pin<port_c, 6>;

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;
        using pd_07 = pin<port_d, 7>;

        // Pin aliases
        using ss_00 = pb_02;
        using mosi_00 = pb_03;
        using miso_00 = pb_04;
        using sck_00 = pb_05;
        using txd_00 = pd_01;
        using rxd_00 = pd_00;
        using xck_00 = pd_04;

        using ports = ::zoal::ct::type_list<port_b, port_c, port_d>;
        using mux = ::zoal::arch::avr::atmega::mux<self_type>;
        using cfg = ::zoal::arch::avr::atmega::cfg<self_type>;
        using irq = ::zoal::arch::avr::atmega::irq;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

namespace zoal { namespace metadata {
    using zoal::ct::integral_constant;
    using atmega328p_sign = zoal::ct::signature<'A', 'T', 'm', 'e', 'g', 'a', '3', '2', '8', 'P'>;

    template<>
    struct pin_to_pwm_channel<atmega328p_sign, 0x0044, 0x0029, 5, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega328p_sign, 0x0044, 0x0029, 6, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega328p_sign, 0x0080, 0x0023, 1, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega328p_sign, 0x0080, 0x0023, 2, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega328p_sign, 0x00B0, 0x0023, 3, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega328p_sign, 0x00B0, 0x0029, 3, 1> : integral_constant<bool, true> {};

    template<>
    struct usart_mapping<atmega328p_sign, 0x00C0, 0x0029, 1> : base_usart_mapping<-1, 0, -1> {};

    template<>
    struct usart_mapping<atmega328p_sign, 0x00C0, 0x0029, 0> : base_usart_mapping<0, -1, -1> {};

    template<>
    struct usart_mapping<atmega328p_sign, 0x00C0, 0x0029, 4> : base_usart_mapping<-1, -1, 0> {};

    template<>
    struct spi_mapping<atmega328p_sign, 0x004C, 0x0023, 2> : base_spi_mapping<-1, -1, -1, 0> {};

    template<>
    struct spi_mapping<atmega328p_sign, 0x004C, 0x0023, 3> : base_spi_mapping<0, -1, -1, -1> {};

    template<>
    struct spi_mapping<atmega328p_sign, 0x004C, 0x0023, 4> : base_spi_mapping<-1, 0, -1, -1> {};

    template<>
    struct spi_mapping<atmega328p_sign, 0x004C, 0x0023, 5> : base_spi_mapping<-1, -1, 0, -1> {};

    template<>
    struct i2c_mapping<atmega328p_sign, 0x00B8, 0x0026, 4> : base_i2c_mapping<0, -1> {};

    template<>
    struct i2c_mapping<atmega328p_sign, 0x00B8, 0x0026, 5> : base_i2c_mapping<-1, 0> {};

    template<>
    struct pin_to_adc_channel<atmega328p_sign, 0x0078, 0x0026, 0> : integral_constant<int, 0> {};

    template<>
    struct pin_to_adc_channel<atmega328p_sign, 0x0078, 0x0026, 1> : integral_constant<int, 1> {};

    template<>
    struct pin_to_adc_channel<atmega328p_sign, 0x0078, 0x0026, 2> : integral_constant<int, 2> {};

    template<>
    struct pin_to_adc_channel<atmega328p_sign, 0x0078, 0x0026, 3> : integral_constant<int, 3> {};

    template<>
    struct pin_to_adc_channel<atmega328p_sign, 0x0078, 0x0026, 4> : integral_constant<int, 4> {};

    template<>
    struct pin_to_adc_channel<atmega328p_sign, 0x0078, 0x0026, 5> : integral_constant<int, 5> {};
}}

#endif

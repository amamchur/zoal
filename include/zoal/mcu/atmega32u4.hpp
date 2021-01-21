/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_ATMEGA32U4_HPP
#define ZOAL_MCU_ATMEGA32U4_HPP

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
    class atmega32u4 {
    public:
        using self_type = atmega32u4;
        using signature = zoal::ct::signature<'A', 'T', 'm', 'e', 'g', 'a', '3', '2', 'U', '4'>;

        template<uintptr_t Address, uint8_t PinMask>
        using port = typename ::zoal::arch::avr::port<Address, PinMask>;

        using port_d = port<0x0029, 0xFF>;
        using port_b = port<0x0023, 0xFF>;
        using port_c = port<0x0026, 0xC0>;
        using port_e = port<0x002C, 0x44>;
        using port_f = port<0x002F, 0xF3>;

        using timer_00 = ::zoal::arch::avr::atmega::timer8<0x0044, 0, false>;
        using timer_01 = ::zoal::arch::avr::atmega::timer16<0x0080, 1>;
        using timer_03 = ::zoal::arch::avr::atmega::timer16<0x0090, 3>;

        using spi_00 = ::zoal::arch::avr::atmega::spi<0x004C, 0>;

        using i2c_00 = ::zoal::arch::avr::atmega::i2c<0x00B8, 0>;

        using usart_01 = typename ::zoal::arch::avr::atmega::usart<0x00C8>;

        using adc_00 = ::zoal::arch::avr::atmega::adc<0x0078, 0>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;
        using pd_07 = pin<port_d, 7>;

        using pb_00 = pin<port_b, 0>;
        using pb_01 = pin<port_b, 1>;
        using pb_02 = pin<port_b, 2>;
        using pb_03 = pin<port_b, 3>;
        using pb_04 = pin<port_b, 4>;
        using pb_05 = pin<port_b, 5>;
        using pb_06 = pin<port_b, 6>;
        using pb_07 = pin<port_b, 7>;

        using pc_06 = pin<port_c, 6>;
        using pc_07 = pin<port_c, 7>;

        using pe_02 = pin<port_e, 2>;
        using pe_06 = pin<port_e, 6>;

        using pf_00 = pin<port_f, 0>;
        using pf_01 = pin<port_f, 1>;
        using pf_04 = pin<port_f, 4>;
        using pf_05 = pin<port_f, 5>;
        using pf_06 = pin<port_f, 6>;
        using pf_07 = pin<port_f, 7>;

        // Pin aliases
        using ss_00 = pb_00;
        using sck_00 = pb_01;
        using mosi_00 = pb_02;
        using miso_00 = pb_03;
        using txd_00 = pd_03;
        using rxd_00 = pd_02;
        using xck_00 = pd_05;
        using rts_00 = pb_07;
        using cts_00 = pd_05;

        using ports = ::zoal::ct::type_list<port_d, port_b, port_c, port_e, port_f>;
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
    using atmega32u4_sign = zoal::ct::signature<'A', 'T', 'm', 'e', 'g', 'a', '3', '2', 'U', '4'>;

    template<>
    struct pin_to_pwm_channel<atmega32u4_sign, 0x0044, 0x0029, 0, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega32u4_sign, 0x0044, 0x0023, 7, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega32u4_sign, 0x0080, 0x0023, 5, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega32u4_sign, 0x0080, 0x0023, 6, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega32u4_sign, 0x0090, 0x0023, 5, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<atmega32u4_sign, 0x0090, 0x0023, 6, 1> : integral_constant<bool, true> {};

    template<>
    struct usart_mapping<atmega32u4_sign, 0x00C8, 0x0000, 0> : base_usart_mapping<0, 0, 0> {};

    template<>
    struct usart_mapping<atmega32u4_sign, 0x00C8, 0x0029, 3> : base_usart_mapping<-1, 0, -1> {};

    template<>
    struct usart_mapping<atmega32u4_sign, 0x00C8, 0x0029, 2> : base_usart_mapping<0, -1, -1> {};

    template<>
    struct usart_mapping<atmega32u4_sign, 0x00C8, 0x0029, 5> : base_usart_mapping<-1, -1, 0> {};

    template<>
    struct spi_mapping<atmega32u4_sign, 0x004C, 0x0023, 0> : base_spi_mapping<-1, -1, -1, 0> {};

    template<>
    struct spi_mapping<atmega32u4_sign, 0x004C, 0x0023, 1> : base_spi_mapping<-1, -1, 0, -1> {};

    template<>
    struct spi_mapping<atmega32u4_sign, 0x004C, 0x0023, 2> : base_spi_mapping<0, -1, -1, -1> {};

    template<>
    struct spi_mapping<atmega32u4_sign, 0x004C, 0x0023, 3> : base_spi_mapping<-1, 0, -1, -1> {};

    template<>
    struct i2c_mapping<atmega32u4_sign, 0x00B8, 0x0029, 1> : base_i2c_mapping<0, -1> {};

    template<>
    struct i2c_mapping<atmega32u4_sign, 0x00B8, 0x0029, 0> : base_i2c_mapping<-1, 0> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x002F, 0> : integral_constant<int, 0> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x002F, 1> : integral_constant<int, 1> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x002F, 4> : integral_constant<int, 4> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x002F, 5> : integral_constant<int, 5> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x002F, 6> : integral_constant<int, 6> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x002F, 7> : integral_constant<int, 7> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x0029, 4> : integral_constant<int, 8> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x0029, 6> : integral_constant<int, 9> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x0029, 7> : integral_constant<int, 10> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x0023, 4> : integral_constant<int, 11> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x0023, 5> : integral_constant<int, 12> {};

    template<>
    struct pin_to_adc_channel<atmega32u4_sign, 0x0078, 0x0023, 6> : integral_constant<int, 13> {};
}}

#endif

/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_ATMEGA2560_HPP
#define ZOAL_MCU_ATMEGA2560_HPP

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
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/gpio/pin.hpp>
#include <zoal/mcu/base_mcu.hpp>

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class atmega2560 : public base_mcu<Frequency, 1> {
    public:
        template<uintptr_t Address, uint8_t PinMask>
        using port = typename ::zoal::arch::avr::port<Address, PinMask>;

        using port_a = port<0x0020, 0xFF>;
        using port_b = port<0x0023, 0xFF>;
        using port_c = port<0x0026, 0xFF>;
        using port_d = port<0x0029, 0xFF>;
        using port_e = port<0x002C, 0xFF>;
        using port_f = port<0x002F, 0xFF>;
        using port_g = port<0x0032, 0x3F>;
        using port_h = port<0x0100, 0xFF>;
        using port_j = port<0x0103, 0xFF>;
        using port_k = port<0x0106, 0xFF>;
        using port_l = port<0x0109, 0xFF>;

        using timer_00 = ::zoal::arch::avr::atmega::timer8<0x0044, 0, false>;
        using timer_01 = ::zoal::arch::avr::atmega::timer16<0x0080, 1>;
        using timer_02 = ::zoal::arch::avr::atmega::timer8<0x00B0, 2, true>;
        using timer_03 = ::zoal::arch::avr::atmega::timer16<0x0090, 3>;
        using timer_04 = ::zoal::arch::avr::atmega::timer16<0x00A0, 4>;
        using timer_05 = ::zoal::arch::avr::atmega::timer16<0x0120, 5>;

        using spi_00 = ::zoal::arch::avr::atmega::spi<0x004C, 0>;

        template<uint8_t BufferSize>
        using i2c_00 = ::zoal::arch::avr::atmega::i2c<0x00B8, 0, BufferSize>;

        using usart_00 = typename ::zoal::arch::avr::atmega::usart<0x00C0>;

        using usart_01 = typename ::zoal::arch::avr::atmega::usart<0x00C8>;

        using usart_02 = typename ::zoal::arch::avr::atmega::usart<0x00D0>;

        using usart_03 = typename ::zoal::arch::avr::atmega::usart<0x0130>;

        using adc_00 = ::zoal::arch::avr::atmega::adc<0x0078, 0>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using pa_00 = pin<port_a, 0>;
        using pa_01 = pin<port_a, 1>;
        using pa_02 = pin<port_a, 2>;
        using pa_03 = pin<port_a, 3>;
        using pa_04 = pin<port_a, 4>;
        using pa_05 = pin<port_a, 5>;
        using pa_06 = pin<port_a, 6>;
        using pa_07 = pin<port_a, 7>;

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
        using pc_07 = pin<port_c, 7>;

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;
        using pd_07 = pin<port_d, 7>;

        using pe_00 = pin<port_e, 0>;
        using pe_01 = pin<port_e, 1>;
        using pe_02 = pin<port_e, 2>;
        using pe_03 = pin<port_e, 3>;
        using pe_04 = pin<port_e, 4>;
        using pe_05 = pin<port_e, 5>;
        using pe_06 = pin<port_e, 6>;
        using pe_07 = pin<port_e, 7>;

        using pf_00 = pin<port_f, 0>;
        using pf_01 = pin<port_f, 1>;
        using pf_02 = pin<port_f, 2>;
        using pf_03 = pin<port_f, 3>;
        using pf_04 = pin<port_f, 4>;
        using pf_05 = pin<port_f, 5>;
        using pf_06 = pin<port_f, 6>;
        using pf_07 = pin<port_f, 7>;

        using pg_00 = pin<port_g, 0>;
        using pg_01 = pin<port_g, 1>;
        using pg_02 = pin<port_g, 2>;
        using pg_03 = pin<port_g, 3>;
        using pg_04 = pin<port_g, 4>;
        using pg_05 = pin<port_g, 5>;

        using ph_00 = pin<port_h, 0>;
        using ph_01 = pin<port_h, 1>;
        using ph_02 = pin<port_h, 2>;
        using ph_03 = pin<port_h, 3>;
        using ph_04 = pin<port_h, 4>;
        using ph_05 = pin<port_h, 5>;
        using ph_06 = pin<port_h, 6>;
        using ph_07 = pin<port_h, 7>;

        using pj_00 = pin<port_j, 0>;
        using pj_01 = pin<port_j, 1>;
        using pj_02 = pin<port_j, 2>;
        using pj_03 = pin<port_j, 3>;
        using pj_04 = pin<port_j, 4>;
        using pj_05 = pin<port_j, 5>;
        using pj_06 = pin<port_j, 6>;
        using pj_07 = pin<port_j, 7>;

        using pk_00 = pin<port_k, 0>;
        using pk_01 = pin<port_k, 1>;
        using pk_02 = pin<port_k, 2>;
        using pk_03 = pin<port_k, 3>;
        using pk_04 = pin<port_k, 4>;
        using pk_05 = pin<port_k, 5>;
        using pk_06 = pin<port_k, 6>;
        using pk_07 = pin<port_k, 7>;

        using pl_00 = pin<port_l, 0>;
        using pl_01 = pin<port_l, 1>;
        using pl_02 = pin<port_l, 2>;
        using pl_03 = pin<port_l, 3>;
        using pl_04 = pin<port_l, 4>;
        using pl_05 = pin<port_l, 5>;
        using pl_06 = pin<port_l, 6>;
        using pl_07 = pin<port_l, 7>;

        // Pin aliases
        using ss_00 = pb_00;
        using mosi_00 = pb_02;
        using miso_00 = pb_03;
        using sck_00 = pb_01;
        using txd_00 = pe_01;
        using rxd_00 = pe_00;
        using xck_00 = pe_02;
        using txd_01 = pe_01;
        using rxd_01 = pe_00;
        using xck_01 = pe_02;
        using txd_02 = pe_01;
        using rxd_02 = pe_00;
        using xck_02 = pe_02;
        using txd_03 = pe_01;
        using rxd_03 = pe_00;
        using xck_03 = pe_02;

        using ports = ::zoal::ct::
            type_list<port_a, port_b, port_c, port_d, port_e, port_f, port_g, port_h, port_j, port_k, port_l>;
        using api = ::zoal::gpio::api<ports>;
        using mux = ::zoal::arch::avr::atmega::mux<api>;
        using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;
        using irq = ::zoal::arch::avr::atmega::irq;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

namespace zoal { namespace metadata {
    using zoal::ct::integral_constant;

    template<>
    struct pin_to_pwm_channel<0x0044, 0x0032, 5, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0044, 0x0023, 7, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0080, 0x0109, 3, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0080, 0x0109, 4, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x00B0, 0x0023, 4, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x00B0, 0x0100, 6, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0090, 0x0109, 3, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0090, 0x0109, 4, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x00A0, 0x0109, 3, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x00A0, 0x0109, 4, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0120, 0x0109, 3, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0120, 0x0109, 4, 1> : integral_constant<bool, true> {};

    template<>
    struct usart_mapping<0x00C0, 0x0000, 0> : base_usart_mapping<0, 0, 0> {};

    template<>
    struct usart_mapping<0x00C0, 0x002C, 1> : base_usart_mapping<-1, 0, -1> {};

    template<>
    struct usart_mapping<0x00C0, 0x002C, 0> : base_usart_mapping<0, -1, -1> {};

    template<>
    struct usart_mapping<0x00C0, 0x002C, 2> : base_usart_mapping<-1, -1, 0> {};
    template<>
    struct usart_mapping<0x00C8, 0x0000, 0> : base_usart_mapping<0, 0, 0> {};

    template<>
    struct usart_mapping<0x00C8, 0x002C, 1> : base_usart_mapping<-1, 0, -1> {};

    template<>
    struct usart_mapping<0x00C8, 0x002C, 0> : base_usart_mapping<0, -1, -1> {};

    template<>
    struct usart_mapping<0x00C8, 0x002C, 2> : base_usart_mapping<-1, -1, 0> {};
    template<>
    struct usart_mapping<0x00D0, 0x0000, 0> : base_usart_mapping<0, 0, 0> {};

    template<>
    struct usart_mapping<0x00D0, 0x002C, 1> : base_usart_mapping<-1, 0, -1> {};

    template<>
    struct usart_mapping<0x00D0, 0x002C, 0> : base_usart_mapping<0, -1, -1> {};

    template<>
    struct usart_mapping<0x00D0, 0x002C, 2> : base_usart_mapping<-1, -1, 0> {};
    template<>
    struct usart_mapping<0x0130, 0x0000, 0> : base_usart_mapping<0, 0, 0> {};

    template<>
    struct usart_mapping<0x0130, 0x002C, 1> : base_usart_mapping<-1, 0, -1> {};

    template<>
    struct usart_mapping<0x0130, 0x002C, 0> : base_usart_mapping<0, -1, -1> {};

    template<>
    struct usart_mapping<0x0130, 0x002C, 2> : base_usart_mapping<-1, -1, 0> {};

    template<>
    struct spi_mapping<0x004C, 0x0023, 0> : base_spi_mapping<-1, -1, -1, 0> {};

    template<>
    struct spi_mapping<0x004C, 0x0023, 2> : base_spi_mapping<0, -1, -1, -1> {};

    template<>
    struct spi_mapping<0x004C, 0x0023, 3> : base_spi_mapping<-1, 0, -1, -1> {};

    template<>
    struct spi_mapping<0x004C, 0x0023, 1> : base_spi_mapping<-1, -1, 0, -1> {};

    template<>
    struct i2c_mapping<0x00B8, 0x0029, 1> : base_i2c_mapping<0, -1> {};

    template<>
    struct i2c_mapping<0x00B8, 0x0029, 0> : base_i2c_mapping<-1, 0> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 0> : integral_constant<int, 0> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 1> : integral_constant<int, 1> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 2> : integral_constant<int, 2> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 3> : integral_constant<int, 3> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 4> : integral_constant<int, 4> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 5> : integral_constant<int, 5> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 6> : integral_constant<int, 6> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x002F, 7> : integral_constant<int, 7> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 0> : integral_constant<int, 8> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 1> : integral_constant<int, 9> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 2> : integral_constant<int, 10> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 3> : integral_constant<int, 11> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 4> : integral_constant<int, 12> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 5> : integral_constant<int, 13> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 6> : integral_constant<int, 14> {};

    template<>
    struct pin_to_adc_channel<0x0078, 0x0106, 7> : integral_constant<int, 15> {};
}}

#endif

/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_ATTINY13A_HPP
#define ZOAL_MCU_ATTINY13A_HPP

#include <stdint.h>
#include <zoal/arch/avr/adc.hpp>
#include <zoal/arch/avr/attiny/cfg.hpp>
#include <zoal/arch/avr/attiny/irq.hpp>
#include <zoal/arch/avr/attiny/metadata.hpp>
#include <zoal/arch/avr/attiny/mux.hpp>
#include <zoal/arch/avr/port.hpp>
#include <zoal/arch/avr/timer16.hpp>
#include <zoal/arch/avr/timer8.hpp>
#include <zoal/arch/enable.hpp>
#include <zoal/arch/power.hpp>
#include <zoal/gpio/base_api.hpp>
#include <zoal/gpio/pin.hpp>
#include <zoal/gpio/port_link.hpp>
#include <zoal/mcu/base_mcu.hpp>

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class attiny13a : public base_mcu<Frequency, 1> {
    public:
        template<uintptr_t Address, uint8_t PinMask>
        using port = typename ::zoal::arch::avr::port<Address, PinMask>;

        using port_b = port<0x0036, 0x3F>;

        class timer_00_mem_model {
        public:
            using word = uint8_t;

            static constexpr ptrdiff_t OCRxB = 0x00;
            static constexpr ptrdiff_t TCCRxA = 0x06;
            static constexpr ptrdiff_t TCNTx = 0x09;
            static constexpr ptrdiff_t TCCRxB = 0x0A;
            static constexpr ptrdiff_t OCRxA = 0x0D;
            static constexpr ptrdiff_t TIFRx = 0x0F;
            static constexpr ptrdiff_t TIMSKx = 0x10;

            static constexpr uint8_t TCCRxA_COMxA = 0xC0;
            static constexpr uint8_t TCCRxA_COMxB = 0x30;
            static constexpr uint8_t TCCRxA_WGMx = 0x03;
            static constexpr uint8_t TCCRxB_FOCxA = 0x80;
            static constexpr uint8_t TCCRxB_FOCxB = 0x40;
            static constexpr uint8_t TCCRxB_WGMx2 = 0x08;
            static constexpr uint8_t TCCRxB_CSx = 0x07;
            static constexpr uint8_t TIFRx_OCFxB = 0x08;
            static constexpr uint8_t TIFRx_OCFxA = 0x04;
            static constexpr uint8_t TIFRx_TOVx = 0x02;
            static constexpr uint8_t TIMSKx_OCIExB = 0x08;
            static constexpr uint8_t TIMSKx_OCIExA = 0x04;
            static constexpr uint8_t TIMSKx_TOIEx = 0x02;
        };

        using timer_00 = ::zoal::arch::avr::timer8<0x0049, 0, false, timer_00_mem_model>;

        class adc_00_mem_model {
        public:
            static constexpr uintptr_t ADCSRBx = 0x00;
            static constexpr uintptr_t ADCx = 0x01;
            static constexpr uintptr_t ADCSRAx = 0x03;
            static constexpr uintptr_t ADMUXx = 0x04;
            static constexpr uintptr_t DIDR0x = 0x11;
        };

        using adc_00 = ::zoal::arch::avr::adc<0x0023, 0, adc_00_mem_model>;

        template<class Port, uint8_t Offset>
        using pin = typename ::zoal::gpio::pin<Port, Offset>;

        using pb_00 = pin<port_b, 0>;
        using pb_01 = pin<port_b, 1>;
        using pb_02 = pin<port_b, 2>;
        using pb_03 = pin<port_b, 3>;
        using pb_04 = pin<port_b, 4>;
        using pb_05 = pin<port_b, 5>;

        using port_chain = typename ::zoal::gpio::chain_builder<port_b>::chain;
        using api = ::zoal::gpio::base_api<port_chain>;
        using mux = ::zoal::arch::avr::attiny::mux<api>;
        using cfg = ::zoal::arch::avr::attiny::cfg<api, Frequency>;
        using irq = ::zoal::arch::avr::attiny::irq;

        template<class... Module>
        using power = ::zoal::arch::power<Module...>;

        template<class... Module>
        using enable = ::zoal::arch::enable<Module...>;
    };
}}

namespace zoal { namespace metadata {
    using zoal::utils::integral_constant;

    template<>
    struct pin_to_pwm_channel<0x0049, 0x0036, 1, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0049, 0x0036, 0, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_adc_channel<0x0023, 0x0036, 5> : integral_constant<int, 0> {};

    template<>
    struct pin_to_adc_channel<0x0023, 0x0036, 2> : integral_constant<int, 1> {};

    template<>
    struct pin_to_adc_channel<0x0023, 0x0036, 4> : integral_constant<int, 2> {};

    template<>
    struct pin_to_adc_channel<0x0023, 0x0036, 3> : integral_constant<int, 3> {};
}}

#endif
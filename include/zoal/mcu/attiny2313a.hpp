/**
 * Autogenerated file. Do not modify manually.
 */
#ifndef ZOAL_MCU_ATTINY2313A_HPP
#define ZOAL_MCU_ATTINY2313A_HPP

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
#include <zoal/ct/type_list.hpp>
#include <zoal/gpio/api.hpp>
#include <zoal/gpio/pin.hpp>
#include <zoal/mcu/base_mcu.hpp>

namespace zoal { namespace mcu {
    template<uint32_t Frequency>
    class attiny2313a : public base_mcu<Frequency, 1> {
    public:
        template<uintptr_t Address, uint8_t PinMask>
        using port = typename ::zoal::arch::avr::port<Address, PinMask>;

        using port_b = port<0x0036, 0xFF>;
        using port_d = port<0x0030, 0x7F>;
        using port_a = port<0x0039, 0x07>;

        template<uint32_t Address>
        class timer_00_mem_model {
        public:
            using word = uint8_t;

            using TCCRxA = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TCNTx = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TCCRxB = zoal::mem::reg<Address + 0x03, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using OCRxA = zoal::mem::reg<Address + 0x06, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TIFRx = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TIMSKx = zoal::mem::reg<Address + 0x09, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using OCRxB = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

            static constexpr uint8_t TCCRxA_COMxA = 0xC0;
            static constexpr uint8_t TCCRxA_COMxB = 0x30;
            static constexpr uint8_t TCCRxA_WGMx = 0x03;
            static constexpr uint8_t TCCRxB_FOCxA = 0x80;
            static constexpr uint8_t TCCRxB_FOCxB = 0x40;
            static constexpr uint8_t TCCRxB_WGMx2 = 0x08;
            static constexpr uint8_t TCCRxB_CSx = 0x07;
            static constexpr uint8_t TIFRx_OCFxB = 0x04;
            static constexpr uint8_t TIFRx_TOVx = 0x02;
            static constexpr uint8_t TIFRx_OCFxA = 0x01;
            static constexpr uint8_t TIMSKx_OCIExB = 0x04;
            static constexpr uint8_t TIMSKx_TOIEx = 0x02;
            static constexpr uint8_t TIMSKx_OCIExA = 0x01;
        };

        using timer_00 = ::zoal::arch::avr::timer8<0x0050, 0, false, timer_00_mem_model<0x0050>>;

        template<uint32_t Address>
        class timer_01_mem_model {
        public:
            using word = uint16_t;

            using TCCRxC = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using ICRx = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using OCRxB = zoal::mem::reg<Address + 0x06, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using OCRxA = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TCNTx = zoal::mem::reg<Address + 0x0A, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TCCRxB = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TCCRxA = zoal::mem::reg<Address + 0x0D, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TIFRx = zoal::mem::reg<Address + 0x16, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
            using TIMSKx = zoal::mem::reg<Address + 0x17, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

            static constexpr uint8_t TCCRxC_FOCxA = 0x80;
            static constexpr uint8_t TCCRxC_FOCxB = 0x40;
            static constexpr uint8_t TCCRxB_ICNCx = 0x80;
            static constexpr uint8_t TCCRxB_ICESx = 0x40;
            static constexpr uint8_t TCCRxB_WGMx = 0x18;
            static constexpr uint8_t TCCRxB_CSx = 0x07;
            static constexpr uint8_t TCCRxA_COMxA = 0xC0;
            static constexpr uint8_t TCCRxA_COMxB = 0x30;
            static constexpr uint8_t TCCRxA_WGMx = 0x03;
            static constexpr uint8_t TIFRx_TOVx = 0x80;
            static constexpr uint8_t TIFRx_OCFxA = 0x40;
            static constexpr uint8_t TIFRx_OCFxB = 0x20;
            static constexpr uint8_t TIFRx_ICFx = 0x08;
            static constexpr uint8_t TIMSKx_TOIEx = 0x80;
            static constexpr uint8_t TIMSKx_OCIExA = 0x40;
            static constexpr uint8_t TIMSKx_OCIExB = 0x20;
            static constexpr uint8_t TIMSKx_ICIEx = 0x08;
        };

        using timer_01 = ::zoal::arch::avr::timer16<0x0042, 1, timer_01_mem_model<0x0042>>;

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

        using pd_00 = pin<port_d, 0>;
        using pd_01 = pin<port_d, 1>;
        using pd_02 = pin<port_d, 2>;
        using pd_03 = pin<port_d, 3>;
        using pd_04 = pin<port_d, 4>;
        using pd_05 = pin<port_d, 5>;
        using pd_06 = pin<port_d, 6>;

        using pa_00 = pin<port_a, 0>;
        using pa_01 = pin<port_a, 1>;
        using pa_02 = pin<port_a, 2>;

        using ports = ::zoal::ct::type_list<port_b, port_d, port_a>;
        using api = ::zoal::gpio::api<ports>;
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
    using zoal::ct::integral_constant;

    template<>
    struct pin_to_pwm_channel<0x0050, 0x0030, 5, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0050, 0x0036, 2, 0> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0042, 0x0036, 4, 1> : integral_constant<bool, true> {};

    template<>
    struct pin_to_pwm_channel<0x0042, 0x0036, 3, 0> : integral_constant<bool, true> {};

}}

#endif

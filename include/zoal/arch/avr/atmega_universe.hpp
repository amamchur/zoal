#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_UNIVERSE_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_UNIVERSE_HPP

#include "adc.hpp"
#include "clock_source.hpp"
#include "atmega/i2c.hpp"
#include "atmega/usart.hpp"
#include "atmega/spi.hpp"
#include "atmega/timer_interrupt_flags_vector.hpp"
#include "atmega/timer_interrupt_mask_vector.hpp"
#include "atmega/mux.hpp"
#include "atmega/cfg.hpp"
#include "port.hpp"
#include "timer_model.hpp"
#include "timer8.hpp"
#include "timer16.hpp"
#include "../../periph/usart_config.hpp"
#include "../../gpio/pin.hpp"

namespace zoal { namespace arch { namespace avr {
    class atmega_universe {
    private:
        using tifrs = avr::timer_interrupt_flags_vector<0x35, 6>;
        using timrs = avr::timer_interrupt_mask_vector<0x6E, 6>;
        using pcs = avr::prescaler_clock_source;
        using pecs = avr::prescaler_and_ext_clock_source;
    public:
        template<uintptr_t Address>
        using port = typename avr::port<Address>;

        using port_a = port<0x0020>;
        using port_b = port<0x0023>;
        using port_c = port<0x0026>;
        using port_d = port<0x0029>;
        using port_e = port<0x002C>;
        using port_f = port<0x002F>;
        using port_g = port<0x0032>;
        using port_h = port<0x0100>;
        using port_j = port<0x0103>;
        using port_k = port<0x0106>;
        using port_l = port<0x0109>;

        using timer8_model = avr::timer8_model<avr::mcu_type::atmega>;
        using timer16_model = avr::timer16_model<avr::mcu_type::atmega>;

        template<uintptr_t Address, class ClockSource, uint8_t N>
        using timer8_type = typename avr::timer8<timer8_model, Address, tifrs, timrs, ClockSource, N>;

        template<uintptr_t Address, class ClockSource, uint8_t N>
        using timer16_type = typename avr::timer16<timer16_model, Address, tifrs, timrs, ClockSource, N>;

        using timer0 = timer8_type<0x44, pecs, 0>;
        using timer1 = timer16_type<0x80, pecs, 1>;
        using timer2 = timer8_type<0xB0, pcs, 2>;
        using timer3 = timer16_type<0x0090, pecs, 3>;
        using timer4 = timer16_type<0x00A0, pecs, 4>;
        using timer5 = timer16_type<0x0124, pecs, 5>;

        using adc0 = avr::adc<avr::mcu_type::atmega, 0x78, 0>;
    };
}}}

#endif

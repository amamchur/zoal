#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_UNIVERSE_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_UNIVERSE_HPP

#include "adc.hpp"
#include "atmega/i2c.hpp"
#include "atmega/usart.hpp"
#include "atmega/spi.hpp"
#include "atmega/timer_interrupt_flags_vector.hpp"
#include "atmega/timer_interrupt_mask_vector.hpp"
#include "atmega/mux.hpp"
#include "atmega/cfg.hpp"
#include "port.hpp"
#include "timer8.hpp"
#include "timer16.hpp"
#include "../../periph/usart_config.hpp"
#include "../../gpio/pin.hpp"

namespace zoal { namespace arch { namespace avr {
    class atmega_universe {
    private:
        using tifrs = avr::timer_interrupt_flags_vector<0x35, 6>;
        using timrs = avr::timer_interrupt_mask_vector<0x6E, 6>;
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

        template<uintptr_t Address, uint8_t N, bool Async = false>
        using timer8_type = typename avr::atmega::timer8<Address, N, Async>;

        template<uintptr_t Address, uint8_t N>
        using timer16_type = typename avr::atmega::timer16<Address, N>;

        using timer0 = timer8_type<0x44, 0>;
        using timer1 = timer16_type<0x80, 1>;
        using timer2 = timer8_type<0xB0, 2, true>;
        using timer3 = timer16_type<0x0090, 3>;
        using timer4 = timer16_type<0x00A0, 4>;
        using timer5 = timer16_type<0x0124, 5>;

        using adc0 = avr::adc<avr::mcu_type::atmega, 0x78, 0>;
    };
}}}

#endif

#ifndef ZOAL_ARCH_ATMEL_AVR_ATTINY_UNIVERSE_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATTINY_UNIVERSE_HPP

#include "attiny/timer_interrupt_flag_register.hpp"
#include "attiny/timer_interrupt_mask_register.hpp"
#include "port.hpp"
#include "clock_source.hpp"
#include "timer_model.hpp"
#include "timer8.hpp"
#include "timer16.hpp"

namespace zoal { namespace arch {
    class attiny_universe {
    public:
        template<uintptr_t Address>
        using port = typename ::zoal::arch::avr::port<Address>;

        typedef port<0x39> port_a;
        typedef port<0x36> port_b;
        typedef port<0x32> port_d;
    };
}}

#endif

#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../mcu/stm32f303xDxE.hpp"
#include "../gpio/base_api.hpp"

namespace zoal { namespace board {
    typedef zoal::mcu::stm32f303xDxE<72000000> mcu;

    // LD1 - power led
    // LD2 - communication led
    typedef mcu::pe08 LD04; // 315 deg, blue
    typedef mcu::pe09 LD03; //   0 deg, red
    typedef mcu::pe10 LD05; //  45 deg, orange
    typedef mcu::pe11 LD07; //  90 deg, green
    typedef mcu::pe14 LD08; // 225 deg, orange
    typedef mcu::pe12 LD09; // 135 deg, blue
    typedef mcu::pe13 LD10; // 180 deg, red
    typedef mcu::pe15 LD06; // 270 deg, green
    typedef mcu::pa00 BuildInUserButton;

    template<class Controller, class Next>
    using aggregation_link = typename ::zoal::gpio::aggregation_link<Controller, Next>;

    class aggregator_chain_builder {
    protected:
        typedef aggregation_link<mcu::port_a, ::zoal::gpio::terminator> Link0;
        typedef aggregation_link<mcu::port_b, Link0> Link1;
        typedef aggregation_link<mcu::port_c, Link1> Link2;
        typedef aggregation_link<mcu::port_d, Link2> Link3;
        typedef aggregation_link<mcu::port_e, Link3> Link4;
    public:
        typedef aggregation_link<mcu::port_f, Link4> Type;
    };

    typedef aggregator_chain_builder::Type gpio_chain;
    typedef ::zoal::gpio::base_api<gpio_chain> api;
}}

#endif

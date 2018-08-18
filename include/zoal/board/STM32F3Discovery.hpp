#ifndef ZOAL_BOARD_HPP
#define ZOAL_BOARD_HPP

#include "../mcu/stm32f303xDxE.hpp"
#include "../gpio/base_api.hpp"

namespace zoal { namespace board {
    using mcu = zoal::mcu::stm32f303xDxE<72000000>;

    // LD1 - power led
    // LD2 - communication led
    using LD04 = mcu::pe08; // 315 deg, blue
    using LD03 = mcu::pe09; //   0 deg, red
    using LD05 = mcu::pe10; //  45 deg, orange
    using LD07 = mcu::pe11; //  90 deg, green
    using LD08 = mcu::pe14; // 225 deg, orange
    using LD09 = mcu::pe12; // 135 deg, blue
    using LD10 = mcu::pe13; // 180 deg, red
    using LD06 = mcu::pe15; // 270 deg, green
    using BuildInUserButton = mcu::pa00;

    template<class Controller, class Next>
    using aggregation_link = typename ::zoal::gpio::aggregation_link<Controller, Next>;

    class aggregator_chain_builder {
    protected:
        using Link0 = aggregation_link<mcu::port_a, ::zoal::gpio::terminator>;
        using Link1 = aggregation_link<mcu::port_b, Link0>;
        using Link2 = aggregation_link<mcu::port_c, Link1>;
        using Link3 = aggregation_link<mcu::port_d, Link2>;
        using Link4 = aggregation_link<mcu::port_e, Link3>;
    public:
        using Type = aggregation_link<mcu::port_f, Link4>;
    };

    using gpio_chain = aggregator_chain_builder::Type;
    using api = ::zoal::gpio::base_api<gpio_chain>;
}}

#endif

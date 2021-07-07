#ifndef ZOAL_GPIO_SOFTWARE_SPI_HPP
#define ZOAL_GPIO_SOFTWARE_SPI_HPP

#include "../ct/constant.hpp"
#include "../gpio/api.hpp"
#include "../gpio/pin_mode.hpp"
#include "../utils/nop.hpp"
#include "spi.hpp"

#include <stdint.h>

namespace zoal { namespace periph {
    template<zoal::periph::spi_cpol_cpha V>
    struct tx_software_polarity : zoal::ct::integral_constant<int, 0> {};

    template<>
    struct tx_software_polarity<zoal::periph::spi_cpol_cpha::mode_2> : zoal::ct::integral_constant<int, 1> {};

    template<>
    struct tx_software_polarity<zoal::periph::spi_cpol_cpha::mode_3> : zoal::ct::integral_constant<int, 1> {};

    template<class Data, class Clock, spi_cpol_cpha PolPha, bit_order Order, uint64_t NoOps>
    class tx_software_spi_transmitter {
    public:
        static constexpr auto polarity = tx_software_polarity<PolPha>::value;

        static void inline transfer_byte(uint8_t data) {
            uint8_t i = 8;
            do {
                Data::write((data >> --i) & 1);
                zoal::utils::nop<NoOps>::place();
                typename Clock::template set<1 - polarity>();
                zoal::utils::nop<NoOps>::place();
                typename Clock::template set<polarity>();
            } while (i > 0);
        }
    };

    template<class Data, class Clock, spi_cpol_cpha PolPha, uint64_t NoOps>
    class tx_software_spi_transmitter<Data, Clock, PolPha, bit_order::lsbf, NoOps> {
    public:
        static constexpr auto polarity = tx_software_polarity<PolPha>::value;

        static void inline transfer_byte(uint8_t data) {
            uint8_t i = 0;
            do {
                Data::write((data >> i++) & 1);
                zoal::utils::nop<NoOps>::place();
                typename Clock::template set<1 - polarity>();
                zoal::utils::nop<NoOps>::place();
                typename Clock::template set<polarity>();
            } while (i < 8);
        }
    };

    template<class Data, class Clock, bit_order Order = bit_order::msbf, spi_cpol_cpha PolPha = spi_cpol_cpha::mode_0, uint64_t NoOps = 0>
    class tx_software_spi : public tx_software_spi_transmitter<Data, Clock, PolPha, Order, NoOps> {
    public:
        static constexpr auto polarity = tx_software_polarity<PolPha>::value;

        using gpio_cfg = zoal::gpio::api::optimize<
            //
            typename Data::port::clock_on_cas,
            typename Clock::port::clock_on_cas,
            zoal::gpio::api::mode<zoal::gpio::pin_mode::output, Data, Clock>,
            typename Data::_0,
            typename Clock::template set<polarity>
            >;
    };
}}

#endif

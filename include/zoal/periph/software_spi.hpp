#ifndef ZOAL_GPIO_SOFTWARE_SPI_HPP
#define ZOAL_GPIO_SOFTWARE_SPI_HPP

#include "../ct/constant.hpp"
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

    template<class Mosi, class Clock, spi_cpol_cpha PolPha, bit_order Order, uint64_t NoOps>
    class tx_software_spi_transmitter {
    public:
        static constexpr auto polarity = tx_software_polarity<PolPha>::value;

        static void inline transfer_byte(uint8_t data) {
            uint8_t i = 8;
            do {
                Mosi::write((data >> --i) & 1);
                zoal::utils::nop<NoOps>::place();
                Clock::write(1 - polarity);
                zoal::utils::nop<NoOps>::place();
                Clock::write(polarity);
            } while (i > 0);
        }
    };

    template<class Mosi, class Clock, spi_cpol_cpha PolPha, uint64_t NoOps>
    class tx_software_spi_transmitter<Mosi, Clock, PolPha, bit_order::lsbf, NoOps> {
    public:
        static constexpr auto polarity = tx_software_polarity<PolPha>::value;

        static void inline transfer_byte(uint8_t data) {
            uint8_t i = 0;
            do {
                Mosi::write((data >> i++) & 1);
                zoal::utils::nop<NoOps>::place();
                Clock::write(1 - polarity);
                zoal::utils::nop<NoOps>::place();
                Clock::write(polarity);
            } while (i < 8);
        }
    };

    template<class Mosi,
             class Clock,
             bit_order Order = bit_order::msbf,
             spi_cpol_cpha PolPha = spi_cpol_cpha::mode_0,
             uint64_t NoOps = 0>
    class tx_software_spi : public tx_software_spi_transmitter<Mosi, Clock, PolPha, Order, NoOps> {
    public:
        static constexpr auto polarity = tx_software_polarity<PolPha>::value;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            using namespace zoal::gpio;
            typename Mosi::template mode<pin_mode::output_push_pull>();
            typename Clock::template mode<pin_mode::output_push_pull>();
            typename Clock::template set<polarity>();
        }

        static void disable() {
            using namespace zoal::gpio;
            typename Mosi::template mode<pin_mode::input_floating>();
            typename Clock::template mode<pin_mode::input_floating>();
        }
    };
}}

#endif

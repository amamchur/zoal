#ifndef ZOAL_GPIO_SOFTWARE_SPI_HPP
#define ZOAL_GPIO_SOFTWARE_SPI_HPP

#include "zoal/utils/nop.hpp"
#include "zoal/gpio/pin_mode.hpp"

#include <stdint.h>

namespace zoal { namespace gpio {
    template<class Mosi, class Clock, uint8_t Polarity, uint64_t NoOps>
    class msbf_tx_software_spi {
    public:
        static void inline transfer_byte(uint8_t data) {
            uint8_t i = 8;
            do {
                Mosi::write((data >> --i) & 1);
                zoal::utils::nop<NoOps>::place();
                Clock::write(1 - Polarity);
                zoal::utils::nop<NoOps>::place();
                Clock::write(Polarity);
            } while (i > 0);
        }

        static void init() {
            using namespace zoal::gpio;
            Mosi::template mode<pin_mode::output>();
            Clock::template mode<pin_mode::output>();
            Clock::write(Polarity);
        }
    };

    template<class Mosi, class Clock, uint8_t Polarity, uint64_t NoOps>
    class lsbf_tx_software_spi {
    public:
        static void inline transfer_byte(uint8_t data) {
            uint8_t i = 0;
            do {
                Mosi::write((data >> i++) & 1);
                zoal::utils::nop<NoOps>::place();
                Clock::write(1 - Polarity);
                zoal::utils::nop<NoOps>::place();
                Clock::write(Polarity);
            } while (i < 8);
        }
        
        static void init() {
            using namespace zoal::gpio;
            Mosi::template mode<pin_mode::output>();
            Clock::template mode<pin_mode::output>();
            Clock::write(Polarity);
        }
    };

    template<class Mosi, class Clock, uint64_t NoOps = 0>
    class tx_software_spi {
    public:
        using msbf0 = msbf_tx_software_spi<Mosi, Clock, 0, NoOps>;
        using msbf1 = msbf_tx_software_spi<Mosi, Clock, 0, NoOps>;
        using msbf2 = msbf_tx_software_spi<Mosi, Clock, 1, NoOps>;
        using msbf3 = msbf_tx_software_spi<Mosi, Clock, 1, NoOps>;

        using lsbf0 = lsbf_tx_software_spi<Mosi, Clock, 0, NoOps>;
        using lsbf1 = lsbf_tx_software_spi<Mosi, Clock, 0, NoOps>;
        using lsbf2 = lsbf_tx_software_spi<Mosi, Clock, 1, NoOps>;
        using lsbf3 = lsbf_tx_software_spi<Mosi, Clock, 1, NoOps>;
    };
}}

#endif

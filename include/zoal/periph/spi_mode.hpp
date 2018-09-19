#ifndef ZOAL_PERIPH_SPI_MODE_HPP
#define ZOAL_PERIPH_SPI_MODE_HPP

namespace zoal { namespace periph {
    enum class spi_mode {
        master,
        slave,
    };

    enum class spi_polarity {
        low,
        high,
    };

    enum class spi_phase {
        low,
        high,
    };

    enum class bit_order {
        msbf,
        lsbf,
    };
}}

#endif

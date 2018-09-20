#ifndef ZOAL_PERIPH_SPI_HPP
#define ZOAL_PERIPH_SPI_HPP

namespace zoal { namespace periph {
    enum class spi_mode {
        master,
        slave,
    };

    enum class spi_cpol_cpha{
        mode_0,
        mode_1,
        mode_2,
        mode_3
    };

    enum class bit_order {
        msbf,
        lsbf,
    };
}}

#endif

#ifndef ZOAL_PERIPH_SPI_HPP
#define ZOAL_PERIPH_SPI_HPP

namespace zoal { namespace periph {
    enum class spi_mode {
        master,
        slave,
    };

    enum class spi_cpol_cpha { mode_0, mode_1, mode_2, mode_3 };

    enum class bit_order {
        msbf,
        lsbf,
    };

    template<uintptr_t ClockDivider = 2, bit_order Order = bit_order::msbf, spi_cpol_cpha PolPha = spi_cpol_cpha::mode_0, spi_mode Mode = spi_mode::master>
    class spi_config {
    public:
        static constexpr uintptr_t clock_divider = ClockDivider;
        static constexpr bit_order order = Order;
        static constexpr spi_cpol_cpha pol_pha = PolPha;
        static constexpr spi_mode mode = Mode;
    };
}}

#endif

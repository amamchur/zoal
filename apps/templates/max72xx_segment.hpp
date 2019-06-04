#ifndef ZOAL_MAX72XX_SEGMENT_HPP
#define ZOAL_MAX72XX_SEGMENT_HPP

#include <zoal/data/segment7.hpp>
#include <zoal/gpio/pin.hpp>
#include <zoal/ic/max72xx.hpp>
#include <zoal/utils/helpers.hpp>

namespace zoal { namespace app {
    template<class Spi, class ChipSelect>
    class max72xx_segment {
    public:
        using matrix_type = zoal::ic::max72xx_data<1>;
        using spi = Spi;
        using max = zoal::ic::max72xx<spi, ChipSelect>;

        uint32_t value{0};

        void init() {
            max::init(matrix_type::devices);
        }

        void run_once() {
            matrix_type matrix;
            matrix.clear();
            auto end = zoal::utils::radix<16>::split(value, &matrix.data[0][0]);
            if (end == &matrix.data[0][0]) {
                end++;
            }

            zoal::utils::apply(zoal::data::segment7::gfed_hex, &matrix.data[0][0], end);
            max::display(matrix);
            value++;
        }
    };
}}

#endif

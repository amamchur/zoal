#ifndef ZOAL_IO_STREAM_HPP
#define ZOAL_IO_STREAM_HPP

#include "stream_functor.hpp"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace io {
    template<uint8_t Value>
    struct stream_value {
        static constexpr uint8_t value = Value;
    };

    template<uint8_t Value>
    struct radix_value {
        static constexpr uint8_t value = Value;
    };

    template<uint8_t Value>
    struct precision_value {
        static constexpr uint8_t value = Value;
    };

    struct setprecision {
        setprecision(uint8_t value)
            : value(value) {}

        uint8_t value;
    };

    struct cursor_position {
        cursor_position(uint8_t row, uint8_t col)
            : row(row)
            , col(col) {}

        uint8_t row;
        uint8_t col;
    };

    struct new_line_cr_lf {
        template<class T>
        static void write() {
            T::send_byte('\r');
            T::send_byte('\n');
        }
    };

    struct modesoff_cr_lf {
        template<class T>
        static void write() {
            T::send_byte('\033');
            T::send_byte('[');
            T::send_byte('m');
            T::send_byte('\r');
            T::send_byte('\n');
        }
    };

    using pos = cursor_position;

    const stream_value<'\n'> endl;
    const radix_value<16> hex;
    const radix_value<10> dec;
    const radix_value<8> oct;
    const precision_value<0> fixed;
}}

#endif

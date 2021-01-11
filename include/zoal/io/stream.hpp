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
            T::push_back_blocking('\r');
            T::push_back_blocking('\n');
        }
    };

    struct modesoff_cr_lf {
        template<class T>
        static void write() {
            T::push_back_blocking('\033');
            T::push_back_blocking('[');
            T::push_back_blocking('m');
            T::push_back_blocking('\r');
            T::push_back_blocking('\n');
        }
    };

    struct buffer_functor : public output_stream_functor<buffer_functor> {
        buffer_functor(const void *buffer, size_t count)
            : buffer((const uint8_t *)buffer)
            , count(count) {}

        template<class T>
        void write() {
            for (size_t i = 0; i < count; i++) {
                T::push_back_blocking(buffer[i]);
            }
        }

        const uint8_t *buffer;
        size_t count;
    };

    struct ignore_functor : public input_stream_functor<ignore_functor> {
        ignore_functor(int count, char ch)
            : count(count)
            , ch(ch) {}

        bool operator()(uint8_t value) {
            return !(--count <= 0 || value == ch);
        }

        int count;
        char ch;
    };

    struct white_space_functor : public input_stream_functor<white_space_functor> {
        bool operator()(uint8_t value) {
            return isspace(value);
        }
    };

    using pos = cursor_position;

    const stream_value<'\n'> endl;
    const radix_value<16> hex;
    const radix_value<10> dec;
    const radix_value<8> oct;
    const precision_value<0> fixed;
    const white_space_functor ws;
}}

#endif

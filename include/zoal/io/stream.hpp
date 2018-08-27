#ifndef ZOAL_IO_STREAM_HPP
#define ZOAL_IO_STREAM_HPP

#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include "stream_functor.hpp"

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
        setprecision(uint8_t value) : value(value) {
        }

        uint8_t value;
    };

    struct cursor_position {
        cursor_position(uint8_t row, uint8_t col) : row(row), col(col) {
        }

        uint8_t row;
        uint8_t col;
    };

    struct new_line_cr_lf : public output_stream_functor<new_line_cr_lf> {
        bool operator()(uint8_t &data) {
            data = static_cast<uint8_t>(index == 0 ? '\r' : '\n');
            return index++ < 2;
        }

        uint8_t index{0};
    };

    struct stop_escape_sequence : public output_stream_functor<stop_escape_sequence> {
        bool operator()(uint8_t &data) {
            switch (index) {
                case 0:
                    data = '\033';
                    break;
                case 1:
                    data = '[';
                    break;
                case 2:
                    data = '0';
                    break;
                case 3:
                    data = 'm';
                    break;
                default:
                    break;
            }
            return index++ < 4;
        }

        uint8_t index{0};
    };

    struct fill_functor : public output_stream_functor<fill_functor> {
        fill_functor(uint8_t ch, size_t count) : ch(ch), count(count) {
        }

        bool operator()(uint8_t &data) {
            data = ch;
            return count-- > 0;
        }

        uint8_t ch{' '};
        size_t count{0};
    };

    struct buffer_functor : public output_stream_functor<buffer_functor> {
        buffer_functor(const void *buffer, size_t count)
                : buffer((const uint8_t *) buffer), count(count) {
        }

        bool operator()(uint8_t &data) {
            data = *buffer++;
            return count-- > 0;
        }

        const uint8_t *buffer;
        size_t count;
    };

    struct string_functor : public output_stream_functor<string_functor> {
        explicit string_functor(const void *buffer)
                : buffer((const uint8_t *) buffer) {
        }

        bool operator()(uint8_t &data) {
            data = *buffer++;
            return data != 0;
        }

        const uint8_t *buffer;
    };


    struct ignore_functor : public input_stream_functor<ignore_functor> {
        ignore_functor(int count, char ch) : count(count), ch(ch) {
        }

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

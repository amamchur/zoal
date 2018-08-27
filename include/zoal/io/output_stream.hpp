#pragma once

#ifndef ZOAL_IO_OUTPUT_STREAM_HPP
#define ZOAL_IO_OUTPUT_STREAM_HPP

#include <stdint.h>
#include <math.h> /* NOLINT */
#include <stdlib.h> /* NOLINT */

#include "stream.hpp"

namespace zoal { namespace io {
    template<class Transport>
    class output_stream {
    public:
        uint8_t radix{10};
        uint8_t precision{2};

        output_stream &operator<<(const char *str) {
            string_functor sf(str);
            Transport::write(sf);
            return *this;
        }

        uint8_t *formatNumber(uint8_t *ptr, uint32_t value) {
            do {
                auto v = static_cast<uint8_t>(value % radix);
                value /= radix;
                v += v < 0xA ? 0x30 : 0x37; // To ASCII
                *--ptr = v;
            } while (value > 0);

            return ptr;
        }

        output_stream &operator<<(uint32_t value) {
            uint8_t buffer[16];
            uint8_t *ptr = buffer + sizeof(buffer);
            ptr = formatNumber(ptr, value);
            auto length = static_cast<size_t>(sizeof(buffer) - (ptr - buffer));
            buffer_functor bf(ptr, length);
            Transport::write(bf);
            return *this;
        }

        output_stream &operator<<(uint16_t value) {
            return *this << (uint32_t) value;
        }

        output_stream &operator<<(uint8_t value) {
            return *this << (uint32_t) value;
        }

        output_stream &operator<<(char value) {
            Transport::write((uint8_t) value);
            return *this;
        }

        output_stream &operator<<(int32_t value) {
            bool negative = value < 0;
            if (negative) {
                value = -value;
                Transport::write('-');
            }

            uint8_t buffer[16];
            uint8_t *ptr = buffer + sizeof(buffer);
            ptr = formatNumber(ptr, static_cast<uint32_t>(value));

            auto length = static_cast<size_t>(sizeof(buffer) - (ptr - buffer));
            buffer_functor bf(ptr, length);
            Transport::write(bf);
            return *this;
        }

        output_stream &operator<<(int16_t value) {
            return *this << (int32_t) value;
        }

        output_stream &operator<<(int8_t value) {
            return *this << (int32_t) value;
        }

        output_stream &operator<<(double value) {
            bool negative = value < 0.0;
            if (negative) {
                value = -value;
                Transport::write('-');
            }

            auto int_part = static_cast<uint32_t>(value);
            double fraction = value - int_part;
            *this << int_part;

            if (precision == 0) {
                return *this;
            }

            Transport::write('.');

            double e = 0.5;
            for (uint8_t i = 0; i < precision; i++) {
                e /= 10;
            }

            fraction += e;

            for (uint8_t i = 0; i < precision; i++) {
                fraction *= 10.0;
                int_part = static_cast<uint32_t>(fraction);
                fraction -= int_part;
                Transport::write(int_part + '0');
            }

            return *this;
        }

        output_stream &operator<<(float value) {
            return *this << (double) value;
        }

        template<uint8_t v>
        output_stream &operator<<(const stream_value<v> &) {
            Transport::write(v);
            return *this;
        }

        template<uint8_t v>
        output_stream &operator<<(const radix_value<v> &) {
            this->radix = v;
            return *this;
        }

        output_stream &operator<<(const setprecision &p) {
            this->precision = p.value;
            return *this;
        }

        output_stream &operator<<(cursor_position pos) {
            Transport::move(pos.row, pos.col);
            return *this;
        }

        template<class T>
        output_stream &operator<<(T t) {
            auto a = t.abstraction();
            Transport::write(a);
            return *this;
        }
    };
}}

#endif

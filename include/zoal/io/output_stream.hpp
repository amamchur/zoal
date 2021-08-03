#pragma once

#ifndef ZOAL_IO_OUTPUT_STREAM_HPP
#define ZOAL_IO_OUTPUT_STREAM_HPP

#include "stream.hpp"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

namespace zoal { namespace io {
    template<class Target>
    class output_stream {
    public:
        explicit output_stream(Target &t)
            : target(t) {}

        output_stream &operator<<(char ch) {
            target.send_byte(static_cast<uint8_t>(ch));
            return *this;
        }

        output_stream &operator<<(const char *str) {
            auto end = str;
            while (*end) end++;
            target.send_data(str, end - str);
            return *this;
        }

        output_stream &operator<<(const void *const ptr) {
            constexpr auto size = sizeof(const void *const) * 2 + 2;
            char buffer[size];
            buffer[0] = '0';
            buffer[1] = 'x';

            auto value = reinterpret_cast<uintptr_t>(ptr);
            for (int i = sizeof(ptr) * 8 - 4, k = 2; i >= 0; i -= 4, k++) {
                auto h = (value & (0xF << i)) >> i;
                buffer[k] = static_cast<char>(h < 10 ? ('0' + h) : ('A' + h - 10));
            }

            target.send_data(buffer, sizeof(buffer));

            return *this;
        }

        output_stream &operator<<(unsigned long value) {
            uint8_t buffer[32];
            uint8_t *ptr = buffer + sizeof(buffer);
            ptr = backward_number_format(ptr, value, radix);

            auto length = static_cast<size_t>(sizeof(buffer) - (ptr - buffer));
            target.send_data(ptr, length);
            return *this;
        }

        output_stream &operator<<(unsigned int value) {
            return *this << (unsigned long)value;
        }

        output_stream &operator<<(unsigned short value) {
            return *this << (unsigned long)value;
        }

        output_stream &operator<<(long value) {
            bool negative = value < 0;
            if (negative) {
                value = -value;
                target.send_byte('-');
            }

            uint8_t buffer[32];
            uint8_t *ptr = buffer + sizeof(buffer);
            ptr = backward_number_format(ptr, static_cast<uint32_t>(value), radix);

            auto length = static_cast<size_t>(sizeof(buffer) - (ptr - buffer));
            target.send_data(ptr, length);
            return *this;
        }

        output_stream &operator<<(int value) {
            return *this << (long)value;
        }

        output_stream &operator<<(short value) {
            return *this << (long)value;
        }

        output_stream &operator<<(double value) {
            bool negative = value < 0.0;
            if (negative) {
                value = -value;
                target.send_byte('-');
            }

            double e = 0.5;
            for (uint8_t i = 0; i < precision; i++) {
                e /= 10;
            }

            value += e;

            auto int_part = static_cast<uint32_t>(value);
            double fraction = value - int_part;
            *this << int_part;

            if (precision == 0) {
                return *this;
            }

            target.send_byte('.');

            for (uint8_t i = 0; i < precision; i++) {
                fraction *= 10.0;
                int_part = static_cast<uint32_t>(fraction);
                fraction -= int_part;
                target.send_byte(static_cast<uint8_t>(int_part + '0'));
            }

            return *this;
        }

        output_stream &operator<<(float value) {
            return *this << (double)value;
        }

        template<uint8_t Value>
        output_stream &operator<<(const radix_value<Value> &) {
            this->radix = Value;
            return *this;
        }

        output_stream &operator<<(const setprecision &p) {
            this->precision = p.value;
            return *this;
        }

        template<class F>
        inline output_stream &operator<<(::zoal::io::output_stream_functor<F> &fn) {
            static_cast<F &>(fn).template write<Target>(target);
            return *this;
        }

        template<class F>
        inline output_stream &operator<<(::zoal::io::output_stream_functor<F> &&fn) {
            static_cast<F &>(fn).template write<Target>(target);
            return *this;
        }

        template<class F>
        inline output_stream &operator<<(::zoal::io::transport_functor<F> &&fn) {
            target.apply_functor(fn);
            return *this;
        }

        uint8_t *backward_number_format(uint8_t *ptr, uint32_t value, uint8_t rdx) {
            do {
                auto v = static_cast<uint8_t>(value % rdx);
                value /= rdx;
                v += v < 0xA ? 0x30 : 0x37; // To ASCII
                *--ptr = v;
            } while (value > 0);

            return ptr;
        }

        uint8_t radix{10};
        uint8_t precision{2};
        Target &target;
    };
}}

#endif

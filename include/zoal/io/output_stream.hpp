#pragma once

#ifndef ZOAL_IO_OUTPUT_STREAM_HPP
#define ZOAL_IO_OUTPUT_STREAM_HPP

#include "stream.hpp"

#include <math.h> /* NOLINT */
#include <stdint.h>
#include <stdlib.h> /* NOLINT */

namespace zoal { namespace io {
    class abstract_transport {
    public:
        virtual void write_byte(uint8_t ch) = 0;
    };

    template<class Transport>
    class transport_proxy : public abstract_transport {
    public:
        void write_byte(uint8_t ch) override {
            Transport::push_back_blocking(ch);
        };

        static abstract_transport &instance() {
            static transport_proxy<Transport> transport;
            return transport;
        }
    };

    class memory_writer : public abstract_transport {
    public:
        memory_writer(void *mem)
            : buffer(reinterpret_cast<uint8_t *>(mem))
            , length(0) {}

        void write_byte(uint8_t ch) override {
            buffer[length++] = ch;
        };

        uint8_t *buffer;
        size_t length;
    };

    class output_stream {
    public:
        uint8_t radix{10};
        uint8_t precision{2};
        abstract_transport &proxy;

        output_stream(abstract_transport &p)
            : proxy(p) {}

        output_stream &operator<<(char ch) {
            proxy.write_byte(static_cast<uint8_t >(ch));
            return *this;
        }

        output_stream &operator<<(const char *str) {
            string_functor sf(str);
            return *this << sf;
        }

        output_stream &operator<<(const void *const ptr) {
            proxy.write_byte('0');
            proxy.write_byte('x');

            auto value = reinterpret_cast<uintptr_t>(ptr);

            for (int i = sizeof(ptr) * 8 - 4; i >= 0; i -= 4) {
                auto hex = (value & (0xF << i)) >> i;
                auto ascii = hex < 10 ? ('0' + hex) : ('A' + hex - 10);
                proxy.write_byte(ascii);
            }

            return *this;
        }

        output_stream &operator<<(unsigned long value) {
            uint8_t buffer[32];
            uint8_t *ptr = buffer + sizeof(buffer);
            ptr = formatNumber(ptr, value);
            auto length = static_cast<size_t>(sizeof(buffer) - (ptr - buffer));
            buffer_functor bf(ptr, length);
            return *this << bf;
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
                proxy.write_byte('-');
            }

            uint8_t buffer[32];
            uint8_t *ptr = buffer + sizeof(buffer);
            ptr = formatNumber(ptr, static_cast<uint32_t>(value));

            auto length = static_cast<size_t>(sizeof(buffer) - (ptr - buffer));
            buffer_functor bf(ptr, length);
            return *this << bf;
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
                proxy.write_byte('-');
            }

            auto int_part = static_cast<uint32_t>(value);
            double fraction = value - int_part;
            *this << int_part;

            if (precision == 0) {
                return *this;
            }

            proxy.write_byte('.');

            double e = 0.5;
            for (uint8_t i = 0; i < precision; i++) {
                e /= 10;
            }

            fraction += e;

            for (uint8_t i = 0; i < precision; i++) {
                fraction *= 10.0;
                int_part = static_cast<uint32_t>(fraction);
                fraction -= int_part;
                proxy.write_byte(static_cast<uint8_t>(int_part + '0'));
            }

            return *this;
        }

        output_stream &operator<<(float value) {
            return *this << (double)value;
        }

        template<uint8_t Value>
        output_stream &operator<<(const stream_value<Value> &) {
            proxy.write_byte(Value);
            return *this;
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

        //        output_stream &operator<<(cursor_position pos) {
        //            proxy.move(pos.row, pos.col);
        //            return *this;
        //        }

        template<class F>
        output_stream &operator<<(::zoal::io::output_stream_functor<F> &fn) {
            uint8_t data = 0;
            while (static_cast<F &>(fn)(data)) {
                proxy.write_byte(data);
            }
            return *this;
        }

    private:
        uint8_t *formatNumber(uint8_t *ptr, uint32_t value) {
            do {
                auto v = static_cast<uint8_t>(value % radix);
                value /= radix;
                v += v < 0xA ? 0x30 : 0x37; // To ASCII
                *--ptr = v;
            } while (value > 0);

            return ptr;
        }
    };
}}

#endif

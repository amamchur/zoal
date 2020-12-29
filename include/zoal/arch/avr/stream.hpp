#ifndef ZOAL_ARCH_ATMEL_AVR_STREAM_HPP
#define ZOAL_ARCH_ATMEL_AVR_STREAM_HPP

#include "zoal/io/stream.hpp"

#include <avr/pgmspace.h>

namespace zoal { namespace io {
    struct progmem_str : public zoal::io::output_stream_functor<progmem_str> {
        explicit progmem_str(const char *str)
            : buffer(str) {}

        template<class T>
        void write() {
            auto p = buffer;
            auto v = pgm_read_byte(p++);
            while (v != 0) {
                T::push_back_blocking(static_cast<uint8_t>(v));
                v = pgm_read_byte(p++);
            }
        }

        const char *buffer;
    };

    struct progmem_str_iter {
        progmem_str_iter(const char *str)
            : ptr_(str) {}

        char operator *() const {
            auto v = pgm_read_byte(ptr_);
            return static_cast<char>(v);
        }

        bool operator==(const progmem_str_iter& iter) const {
            return ptr_ == iter.ptr_;
        }

        bool operator!=(const progmem_str_iter& iter) const {
            return ptr_ != iter.ptr_;
        }

        progmem_str_iter& operator++() {
            ptr_++;
            return *this;
        }

        progmem_str_iter operator+(intptr_t d) const {
            return {ptr_ + d};
        }

        intptr_t operator-(progmem_str_iter iter) const {
            return ptr_ - iter.ptr_;
        }

        const char *ptr_;
    };
}}

#endif

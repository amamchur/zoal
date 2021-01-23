#ifndef ZOAL_MEM_TRANSPORT_HPP
#define ZOAL_MEM_TRANSPORT_HPP

#include <cstdint>
#include <cstddef>

#include <zoal/io/output_stream.hpp>

namespace zoal { namespace tests {
    struct mem_transport_clear_functor : zoal::io::transport_functor<mem_transport_clear_functor> {
        template<class L>
        inline void apply(L &mb) {
            mb.ptr = mb.buffer;
            mb.end = mb.buffer + mb.size;
            *mb.ptr = 0;
        }
    };

    class mem_transport {
    public:
        mem_transport(void *b, size_t size)
            : buffer(reinterpret_cast<uint8_t *>(b))
            , ptr(buffer)
            , size(size)
            , end(buffer + size) {}

        void send_byte(uint8_t value) {
            *ptr++ = value;
        }

        void send_data(const void *data, size_t size) {
            const auto *p = reinterpret_cast<const uint8_t *>(data);
            while (size > 0) {
                *ptr++ = *p++;
                size--;
            }
        }

        template<class F>
        inline void apply_functor(::zoal::io::transport_functor<F> &fn) {
            static_cast<F &>(fn).apply(*this);
        }

        uint8_t *buffer;
        uint8_t *ptr;
        size_t size;
        uint8_t *end;
    };
}}

#endif

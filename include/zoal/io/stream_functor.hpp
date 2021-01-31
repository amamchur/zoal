#ifndef ZOAL_STREAM_FUNCTOR_HPP
#define ZOAL_STREAM_FUNCTOR_HPP

namespace zoal { namespace io {
    template<class T>
    struct output_stream_functor {};

    template<class T>
    struct transport_functor {};

    template<class V>
    struct hexadecimal_functor : public zoal::io::output_stream_functor<hexadecimal_functor<V>> {
        explicit hexadecimal_functor(V v)
            : value(v) {}

        template<class T>
        void write(T &t) {
            constexpr uint8_t nibbles = sizeof(V) << 1;

            t.send_byte('0');
            t.send_byte('x');
            for (int i = nibbles - 1; i >= 0; i--) {
                auto h = (value >> (i << 2)) & 0xF;
                uint8_t ch = h < 10 ? ('0' + h) : ('A' + h - 10);
                t.send_byte(ch);
            }
        }

        V value;
    };

    template<class T>
    hexadecimal_functor<T> hexadecimal(T v) {
        return hexadecimal_functor<T>(v);
    }
}}

#endif

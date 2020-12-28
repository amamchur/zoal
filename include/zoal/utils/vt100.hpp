#ifndef ZOAL_UTILS_VT100_HPP
#define ZOAL_UTILS_VT100_HPP

#include "../io/stream.hpp"

#include <stdint.h>

namespace zoal { namespace utils { namespace vt100 {
    const auto none = -1;
    const auto black = 0;
    const auto red = 1;
    const auto green = 2;
    const auto yellow = 3;
    const auto blue = 4;
    const auto magenta = 5;
    const auto cyan = 6;
    const auto white = 7;

    template<class T>
    struct helper {
        static uint8_t *format(uint8_t *ptr, int8_t value) {
            do {
                auto v = static_cast<uint8_t>(value % 10);
                value /= 10;
                *--ptr = v + 0x30;
            } while (value > 0);

            return ptr;
        }

        static void output(uint8_t *ptr) {
            while (*ptr) {
                T::push_back_blocking(*ptr++);
            }
        }
    };

    struct cr_lf : public zoal::io::output_stream_functor<cr_lf> {
        template<class T>
        void write() {
            T::push_back_blocking('\r');
            T::push_back_blocking('\n');
        }
    };

    // Reset terminal to initial state
#if 0
    struct ris : public zoal::io::output_stream_functor<ris> {
        template<class T>
        void write() {
            T::push_back_blocking('\033');
            T::push_back_blocking('c');
        }
    };

    // turn_off_character_attributes
    struct sgr0 : public zoal::io::output_stream_functor<sgr0> {
        template<class T>
        void write() {
            T::push_back_blocking('\033');
            T::push_back_blocking('[');
            T::push_back_blocking('m');
        }
    };

    //  Clear entire line
    struct el2 : public zoal::io::output_stream_functor<el2> {
        template<class T>
        void write() {
            T::push_back_blocking('\033');
            T::push_back_blocking('[');
            T::push_back_blocking('2');
            T::push_back_blocking('k');
        }
    };
#else
    struct ed2 {
        inline operator const char *() const {
            return "\0332j";
        }
    };

    struct ris {
        inline operator const char *() const {
            return "\033c";
        }
    };

    // turn_off_character_attributes
    struct sgr0 {
        inline operator const char *() const {
            return "\033[m";
        }
    };

    struct el2 {
        inline operator const char *() const {
            return "\033[2K";
        }
    };
#endif

    struct color : public zoal::io::output_stream_functor<color> {
        color(int f, int b)
            : foreground(f)
            , background(b) {}

        color(const color &c)
            : foreground(c.foreground)
            , background(c.background) {}

        color() {}

        inline color f(uint8_t value) const {
            return color(value, background);
        }

        inline color b(uint8_t value) const {
            return color(foreground, value);
        }

        template<class T>
        void write() {
            uint8_t buffer[4] = {0};
            T::push_back_blocking('\033');
            T::push_back_blocking('[');

            auto p = buffer + sizeof(buffer) - 1;
            p = helper<T>::format(p, background != none ? 40 + background : 0);
            helper<T>::output(p);

            T::push_back_blocking(';');
            p = buffer + sizeof(buffer) - 1;
            p = helper<T>::format(p, foreground != none ? 30 + foreground : 0);
            helper<T>::output(p);

            T::push_back_blocking('m');
        }

        int8_t foreground{none};
        int8_t background{none};
    };

    // ^[[<v>;<h>H
    struct cup : public zoal::io::output_stream_functor<cup> {
        cup() {}

        cup(int v, int h)
            : vert(v)
            , hor(h) {}

        template<class T>
        void write() {
            uint8_t buffer[4] = {0};
            T::push_back_blocking('\033');
            T::push_back_blocking('[');

            auto p = buffer + sizeof(buffer) - 1;
            p = helper<T>::format(p, vert);
            helper<T>::output(p);

            T::push_back_blocking(';');
            p = buffer + sizeof(buffer) - 1;
            p = helper<T>::format(p, hor);
            helper<T>::output(p);

            T::push_back_blocking('H');
        }

        int8_t vert{0};
        int8_t hor{0};
    };

    using reset_terminal = ris;
    using turn_off_character_attributes = sgr0;
    using clear_entire_line = el2;
    using clear_entire_screen = ed2;
    using cursor_position = cup;
}}}

#endif

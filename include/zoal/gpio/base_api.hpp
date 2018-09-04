#pragma once

#ifndef ZOAL_GPIO_BASE_API_HPP
#define ZOAL_GPIO_BASE_API_HPP

#include "pin_mode.hpp"

namespace zoal { namespace gpio {
    template<zoal::gpio::pin_mode PinMode>
    class mode_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            Link::port::template mode<PinMode>(link.mask);
        }
    };

    class low_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            Link::port::low(link.mask);
        }
    };

    class high_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            Link::port::high(link.mask);
        }
    };

    class toggle_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            Link::port::toggle(link.mask);
        }
    };

    class write_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            auto hb = link.data & link.mask;
            auto lb = link.mask & ~link.data;
            if (lb) {
                Link::port::low(lb);
            }

            if (hb) {
                Link::port::high(hb);
            }
        }
    };

    class power_on_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            if (link.mask) {
                Link::port::power_on();
            }
        }
    };

    class power_off_functor {
    public:
        template<class Link>
        static inline void flush(const Link &link) {
            if (link.mask) {
                Link::port::power_off();
            }
        }
    };

    template<class Chain>
    class base_api {
    public:
        using chain = Chain;

        template<pin_mode PinMode>
        static inline void mode(const chain &&chain) {
            chain.template flush<mode_functor<PinMode>>();
        }

        static inline void low(const chain &&chain) {
            chain.template flush<low_functor>();
        }

        static inline void high(const chain &&chain) {
            chain.template flush<high_functor>();
        }

        static inline void toggle(const chain &&chain) {
            chain.template flush<toggle_functor>();
        }

        static inline void write(const chain &&chain) {
            chain.template flush<write_functor>();
        }

        static inline void power_on(const chain &&chain) {
            chain.template flush<power_on_functor>();
        }

        static inline void power_off(const chain &&chain) {
            chain.template flush<power_off_functor>();
        }
    };
}}

#endif

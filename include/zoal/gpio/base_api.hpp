#pragma once

#ifndef ZOAL_GPIO_BASE_API_HPP
#define ZOAL_GPIO_BASE_API_HPP

#include "pin_mode.hpp"

namespace zoal { namespace gpio {
    template<zoal::gpio::pin_mode PinMode>
    class ModeFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &c) {
            Iterator::port::template mode<PinMode>(c.mask);
        }
    };

    class LowFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &c) {
            Iterator::port::low(c.mask);
        }
    };

    class HighFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &c) {
            Iterator::port::high(c.mask);
        }
    };

    class ToggleFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &c) {
            Iterator::port::toggle(c.mask);
        }
    };

    class WriteFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &c) {
            auto hb = c.data & c.mask;
            auto lb = c.mask & ~c.data;
            if (lb) {
                Iterator::port::low(lb);
            }

            if (hb) {
                Iterator::port::high(hb);
            }
        }
    };

    class EnableFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &) {
            Iterator::port::enable();
        }
    };

    class DisableFunctor {
    public:
        template<class Iterator>
        static inline void flush(const Iterator &) {
            Iterator::port::disable();
        }
    };

    template<class Chain>
    class base_api {
    public:
        using chain = Chain;

        template<pin_mode PinMode>
        static inline void mode(const chain &chain) {
            chain.template flush<ModeFunctor<PinMode>>();
        }

        static inline void low(const chain &chain) {
            chain.template flush<LowFunctor>();
        }

        static inline void high(const chain &chain) {
            chain.template flush<HighFunctor>();
        }

        static inline void toggle(const chain &chain) {
            chain.template flush<ToggleFunctor>();
        }

        static inline void write(const chain &chain) {
            chain.template flush<WriteFunctor>();
        }

        static inline void enable(const chain &chain) {
            chain.template flush<EnableFunctor>();
        }

        static inline void disable(const chain &chain) {
            chain.template flush<DisableFunctor>();
        }
    };
}}

#endif

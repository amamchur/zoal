#pragma once

#ifndef ZOAL_GPIO_PIN_HPP
#define ZOAL_GPIO_PIN_HPP

#include "../ct/helpers.hpp"
#include "../utils/defs.hpp"
#include "./api.hpp"
#include "./features.hpp"
#include "./null_port.hpp"
#include "./pin_mode.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace gpio {
    using zoal::gpio::api;
    using zoal::gpio::features;
    using zoal::gpio::has_features;
    using zoal::mem::null_cas_list;

    template<class Port, uint8_t Offset, class... Mixin>
    class pin : public Mixin... {
    public:
        using port = Port;

        static constexpr bool not_pin = port::address == 0;
        static constexpr uint8_t offset = Offset;
        static constexpr typename port::register_type mask = not_pin ? 0 : 1u << Offset;

        pin() = delete;

        ZOAL_INLINE_IO static uint8_t read() {
            return (port::read() >> offset) & 1u;
        }

        ZOAL_INLINE_IO static void write(uint8_t value) {
            if (value) {
                typename port::template high_cas<mask>();
            } else {
                typename port::template low_cas<mask>();
            }
        }

        ZOAL_INLINE_IO static void toggle() {
            port::template toggle<mask>();
        }

        using _0 = typename port::template low_cas<mask>;
        using _1 = typename port::template high_cas<mask>;
        using low = typename port::template low_cas<mask>;
        using high = typename port::template high_cas<mask>;

        template<uint8_t Value>
        using set = typename zoal::ct::conditional_type<Value == 0, _0, _1>::type;

        template<pin_mode PinMode>
        using mode = typename port::template mode_cas<PinMode, mask>;
    };

    class null_pin : public pin<null_port, 0> {};

    template<class Pin, bool HasOutputOpenDrainFeature>
    class active_drain_strategy {
    public:
        using on = typename zoal::gpio::api::optimize<
            //
            typename Pin::template mode<pin_mode::output>,
            typename Pin::low>;
        using off = typename Pin::template mode<pin_mode::input_floating>;
        using gpio_cfg = off;
    };

    template<class Pin>
    class active_drain_strategy<Pin, true> {
    public:
        using on = typename Pin::high;
        using off = typename Pin::low;
        using gpio_cfg = typename Pin::template mode<pin_mode::output_open_drain>;
    };

    template<class Pin>
    class active_drain {
    public:
        using strategy = active_drain_strategy<Pin, has_features<typename Pin::port, features::output_open_drain>::value>;
        using pin = Pin;
        using on = typename strategy::on;
        using off = typename strategy::off;
        using gpio_cfg = typename api::optimize<
            //
            typename pin::port::clock_on_cas,
            typename strategy::gpio_cfg,
            off>;
    };

    template<class Pin>
    class active_high {
    public:
        using pin = Pin;
        using on = typename Pin::high;
        using off = typename Pin::low;
        using gpio_cfg = typename api::optimize<
            //
            typename pin::port::clock_on_cas,
            typename pin::template mode<pin_mode::output>,
            off>;
    };

    template<class Pin>
    class active_low {
    public:
        using pin = Pin;
        using on = typename Pin::low;
        using off = typename Pin::high;
        using gpio_cfg = typename api::optimize<
            //
            typename pin::port::clock_on_cas,
            typename pin::template mode<pin_mode::output>,
            off>;
    };
}}

#endif

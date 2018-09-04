#pragma once

#ifndef ZOAL_IO_BUTTON_HPP
#define ZOAL_IO_BUTTON_HPP

#include "../gpio/pin_mode.hpp"
#include "../utils/method_invoker.hpp"
#include "button_state_machine.hpp"

namespace zoal { namespace io {

    template<
            zoal::gpio::pin_mode Mode,
            uint8_t DebounceDelay = 5,
            uint16_t PressDelay = 250,
            uint16_t DecPressDelay = 5,
            uint16_t MinPressDelay = 25
    >
    class button_config {
    public:
        static constexpr zoal::gpio::pin_mode mode = Mode;
        static constexpr uint16_t debounce_delay = DebounceDelay;
        static constexpr uint16_t press_delay = PressDelay;
        static constexpr uint16_t dec_press_delay = DecPressDelay;
        static constexpr uint16_t min_press_delay = MinPressDelay;
    };

    using pull_up_button = button_config<zoal::gpio::pin_mode::input_pull_up, 5, 250>;
    using pull_up_button_no_press = button_config<zoal::gpio::pin_mode::input_pull_up, 5, 0>;

    template<class CounterType>
    class button_base {
    protected:
        template<class Callback>
        static void invokeCallback(Callback callback, uint8_t events) {
            if ((events & button_state_trigger_down) != 0) {
                callback(button_event::down);
            }

            if ((events & button_state_trigger_press) != 0) {
                callback(button_event::press);
            }

            if ((events & button_state_trigger_up) != 0) {
                callback(button_event::up);
            }
        }

        CounterType prev_time{0};
        uint8_t button_state{0};
    };

    template<class Pin, class Counter, class Cfg = pull_up_button>
    class button : public button_base<typename Counter::value_type> {
    public:
        using pin = Pin;

        void begin() {
            pin::template mode<Cfg::mode>();
        }

        template<class Callback>
        void handle(Callback callback) {
            using namespace zoal::gpio;

            button_state_machine machine(Cfg::debounce_delay, Cfg::press_delay);
            auto now = Counter::now();
            auto dt = now - this->prev_time;
            auto value = Cfg::mode == pin_mode::input_pull_up ? pin::read() ^ 1u : pin::read();
            auto state = machine.handle_button(dt, this->button_state, static_cast<uint8_t>(value));
            auto events = state & button_state_trigger;
            this->button_state = state & ~button_state_trigger;

            if (events == 0) {
                return;
            }

            this->prev_time = now;

            button::invokeCallback(callback, events);
        }

        template<class T, class M>
        void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, button_event>(obj, m));
        }
    };

    template<class Pin, class Counter, class Config = pull_up_button>
    class button_ext : public button_base<typename Counter::value_type> {
    public:
        using pin = Pin;

        void begin() {
            pin::template mode<Config::mode>();
        }

        template<class Callback>
        void handle(Callback callback) {
            using namespace zoal::gpio;

            button_state_machine machine(Config::debounce_delay, press_delay_ms);
            auto now = Counter::now();
            auto dt = now - this->prev_time;
            auto value = Config::mode == pin_mode::input_pull_up ? pin::read() ^ 1u : pin::read();
            auto state = machine.handle_button(dt, this->button_state, static_cast<uint8_t>(value));
            auto events = state & button_state_trigger;
            this->button_state = state & ~button_state_trigger;

            if (events == 0) {
                return;
            }

            this->prev_time = now;

            if (events & button_state_trigger_down) {
                callback(button_event::down);
            }

            if (events & button_state_trigger_press) {
                callback(button_event::press);
                if (press_delay_ms > Config::min_press_delay) {
                    press_delay_ms -= Config::dec_press_delay;
                }
            }

            if (events & button_state_trigger_up) {
                callback(button_event::up);
                press_delay_ms = Config::press_delay;
            }
        }

        template<class T, class M>
        inline void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, button_event>(obj, m));
        }

    private:
        uint16_t press_delay_ms{Config::press_delay};
    };
}}

#endif

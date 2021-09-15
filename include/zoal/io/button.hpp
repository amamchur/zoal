#ifndef ZOAL_IO_BUTTON_HPP
#define ZOAL_IO_BUTTON_HPP

#include "../gpio/pin_mode.hpp"
#include "../utils/method_invoker.hpp"
#include "button_state_machine.hpp"

namespace zoal { namespace io {

    template<bool ActiveLow,
             uint8_t DebounceDelay = 5,
             uint16_t ReleaseDelay = 500,
             uint16_t PressInterval = 30,
             uint16_t DecPressDelay = 25,
             uint16_t MinPressDelay = 25>
    class button_config {
    public:
        static constexpr bool active_low = ActiveLow;
        static constexpr uint16_t debounce_delay = DebounceDelay;
        static constexpr uint16_t release_delay = ReleaseDelay;
        static constexpr uint16_t repeat_interval = PressInterval;
        static constexpr uint16_t dec_repeat_interval = DecPressDelay;
        static constexpr uint16_t min_repeat_interval = MinPressDelay;

        using machine = button_state_machine;
    };

    using active_low_button = button_config<true, 5, 500, 30>;
    using active_high_button = button_config<false, 5, 500, 30>;
    using active_low_no_press = button_config<true, 5, 0, 0>;
    using active_high_no_press = button_config<false, 5, 0, 0>;

    template<class TimeType, class Config>
    class base_button {
    public:
        using machine_type = typename Config::machine;

        machine_type &machine() {
            return machine_;
        }

    protected:
        machine_type machine_;
        TimeType prev_time{0};
    };

    template<class TimeType, class Pin, class Config = active_low_button>
    class button : public base_button<TimeType, Config> {
    public:
        using pin = Pin;
        using machine_type = typename Config::machine;
        using state_type = typename machine_type::state_type;

        void handle(TimeType current_time) {
            using namespace zoal::gpio;

            auto dt = current_time - this->prev_time;
            constexpr auto debounce = static_cast<typeof(dt)>(Config::debounce_delay);
            constexpr auto release = static_cast<typeof(dt)>(Config::release_delay);
            constexpr auto press = static_cast<typeof(dt)>(Config::repeat_interval);
            uint8_t value = Config::active_low ? pin::read() ^ 1u : pin::read();
            auto switched = machine.handle(value, dt, debounce, release, press);

            if (switched) {
                this->prev_time = current_time;
            }
        }

        template<class Callback, class... Args>
        void handle(TimeType current_time, Callback callback, Args... args) {
            handle(current_time);
            machine.invoke_callback(callback, args...);
        }
    protected:
        machine_type machine;
    };

    template<class TimeType, class Pin, class Config = active_low_button>
    class button_ext : public base_button<TimeType, Config> {
    public:
        using pin = Pin;
        using machine_type = typename Config::machine;
        using state_type = typename machine_type::state_type;

        void handle(TimeType current_time) {
            using namespace zoal::gpio;

            auto dt = current_time - this->prev_time;
            auto value = Config::active_low ? pin::read() ^ 1u : pin::read();
            constexpr auto debounce = static_cast<typeof(dt)>(Config::debounce_delay);
            constexpr auto release = static_cast<typeof(dt)>(Config::release_delay);
            auto switched = this->machine_.handle(static_cast<uint8_t>(value), dt, debounce, release, static_cast<typeof(dt)>(repeat_interval));

            if (switched) {
                this->prev_time = current_time;
            }

            this->machine_.invoke_callback([&](button_event e) {
                switch (e) {
                case button_event::press:
                    if (repeat_interval > Config::min_repeat_interval) {
                        repeat_interval -= Config::dec_repeat_interval;
                    }
                    break;
                case button_event::up:
                    repeat_interval = Config::repeat_interval;
                    break;
                default:
                    break;
                }
            });
        }

        template<class Callback, class... Args>
        void handle(TimeType current_time, Callback callback, Args... args) {
            handle(current_time);
            this->machine_.invoke_callback(callback, args...);
        }
    protected:
        uint16_t repeat_interval{Config::press_delay};
    };
}}

#endif

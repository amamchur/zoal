#pragma once

#ifndef ZOAL_IO_BUTTON_STATE_MACHINE_HPP
#define ZOAL_IO_BUTTON_STATE_MACHINE_HPP

#include "button_state_machine.hpp"

#include <stdint.h>

namespace zoal { namespace io {
    enum class button_event : uint8_t { down, press, up };

    class button_state_machine {
    public:
        enum state_transition {
            fl = 0 << 0, // transition from low (source state)
            fh = 1 << 0, // transition from high (source state)
            tl = 0 << 4, // transition to low (source signal)
            th = 1 << 4, // transition to high (source signal)
            debounce = 1 << 1, // debounce state will be reset after debounce_delay
            pressing = 1 << 2, // pressing state repeat key_press event after press_delay
            pause = 1 << 3,

            switched = 1 << 4, // machine was switched, calling party must reset dt timer value
            key_down = 1 << 5,
            key_press = 1 << 6,
            key_up = 1 << 7,
            all_events = switched | key_down | key_press | key_up
        };

        using state_type = uint8_t;

        template<class T>
        bool handle(uint8_t signal, T dt, T debounce_delay, T release_delay, T repeat_interval) {
            auto transition = static_cast<state_type>((state_ & 0x0Fu) | (signal << 4u));
            auto next = state_ & ~all_events;

            if ((transition & debounce) && (dt >= debounce_delay)) {
                transition &= ~debounce;
            }

            switch (transition) {
            case (fl | th):
                next = fh | debounce | key_down | key_press | switched;
                break;
            case (fh | th):
                if (release_delay != 0 && dt >= release_delay) {
                    next = fh | pressing | key_press | switched;
                }
                break;
            case (fh | th | pressing):
                if (repeat_interval != 0 && dt >= repeat_interval) {
                    next = fh | pressing | key_press | switched;
                }
                break;
            case (fh | tl):
            case (fh | tl | pressing):
                next = fl | key_up | debounce | switched;
                break;
            default:
                break;
            }

            state_ = next;
            return (next & switched) != 0;
        }

        template<class Callback, class ...Args>
        void invoke_callback(Callback callback, Args... args) {
            if ((state_ & button_state_machine::key_down) != 0) {
                callback(button_event::down, args...);
            }

            if ((state_ & button_state_machine::key_press) != 0) {
                callback(button_event::press, args...);
            }

            if ((state_ & button_state_machine::key_up) != 0) {
                callback(button_event::up, args...);
            }
        }

        state_type state() const {
            return state_;
        }

    protected:
        state_type state_{0};
    };
}}

#endif

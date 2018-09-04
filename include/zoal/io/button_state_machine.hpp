#pragma once

#ifndef ZOAL_IO_BUTTON_STATE_MACHINE_HPP
#define ZOAL_IO_BUTTON_STATE_MACHINE_HPP

#include <stdint.h>
#include "button_state_machine.hpp"

namespace zoal { namespace io {
    enum class button_event : uint8_t {
        down,
        press,
        up
    };

    enum button_state : uint8_t {
        button_state_current = 1 << 0,
        button_state_next = 1 << 1,
        button_state_trigger_down = 1 << 2,
        button_state_trigger_press = 1 << 3,
        button_state_trigger_up = 1 << 4,
        button_state_trigger = button_state_trigger_down | button_state_trigger_press | button_state_trigger_up,
        button_state_pressing = 1 << 5,
        button_state_debounce = 1 << 6,
        button_state_pressed = 1 << 7
    };

    class button_state_machine {
    public:
        button_state_machine(uint16_t debounceDelay, uint16_t pressDelay)
                : debounce_delay(debounceDelay), press_delay(pressDelay) {
        }

        template<class T>
        uint8_t handle_button(T dt, uint8_t state, uint8_t value) {
            if (state & button_state_debounce) {
                return handle_debounce(dt, state);
            }

            return handle_press(dt, state, value);
        }

    private:
        uint16_t debounce_delay;
        uint16_t press_delay;

        template<class T>
        inline uint8_t handle_press(T dt, uint8_t state, uint8_t value) {
            auto transition = static_cast<uint8_t>(((state & 0x03) << 4) | value);
            auto next = state;
            switch (transition) {
            case 0x00:
                // No changes. Idle state
                return state;
            case 0x01:
                // Idle->Debounce->Down
                next = button_state_debounce | 1;
                break;
            case 0x11:
                // Debounce->Down
                return button_state_trigger_down | button_state_pressing | 2;
            case 0x20:
                // Down->Debounce->Pressed
                next |= button_state_debounce | 3;
                break;
            case 0x30:
                // Pressed->Idle
                if ((state & button_state_pressed) == 0) {
                    return button_state_trigger_press | button_state_trigger_up;
                }
                return button_state_trigger_up;
            default:
                break;
            }

            if (press_delay == 0) {
                return next;
            }

            bool pressing = (next & button_state_pressing) != 0;
            bool condition = dt >= press_delay && value == 1;
            if (pressing && condition) {
                next |= button_state_trigger_press | button_state_pressed;
            }

            return next;
        }

        template<class T>
        inline uint8_t handle_debounce(T dt, uint8_t state) {
            if (dt >= debounce_delay) {
                state &= ~button_state_debounce;
            }
            return state;
        }
    };
}}

#endif

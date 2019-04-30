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
        button_state_current = 1u << 0u,
        button_state_next = 1u << 1u,
        button_state_trigger_down = 1u << 2u,
        button_state_trigger_press = 1u << 3u,
        button_state_trigger_up = 1u << 4u,
        button_state_trigger = button_state_trigger_down | button_state_trigger_press | button_state_trigger_up,
        button_state_pressing = 1u << 5u,
        button_state_debounce = 1u << 6u,
        button_state_pressed = 1u << 7u
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
            auto transition = static_cast<uint8_t>(((state & 0x03u) << 4u) | value);
            auto next = state;
            switch (transition) {
            case 0x00:
                // No changes. Idle state
                return state;
            case 0x01:
                // Idle->Debounce->Down
                next = button_state_debounce | 1u;
                break;
            case 0x11:
                // Debounce->Down
                return button_state_trigger_down | button_state_pressing | 2u;
            case 0x20:
                // Down->Debounce->Pressed
                next |= button_state_debounce | 3u;
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
            bool condition = dt >= (T)press_delay && value == 1;
            if (pressing && condition) {
                next |= button_state_trigger_press | button_state_pressed;
            }

            return next;
        }

        template<class T>
        inline uint8_t handle_debounce(T dt, uint8_t state) {
            if (dt >= (T)debounce_delay) {
                state &= ~button_state_debounce;
            }
            return state;
        }
    };
}}

#endif

#ifndef ZOAL_IO_ROTARY_ENCODER_HPP
#define ZOAL_IO_ROTARY_ENCODER_HPP

#include "../gpio/pin_mode.hpp"
#include "../utils/method_invoker.hpp"

#include <stdint.h>

namespace zoal { namespace io {
    enum class rotary_event : uint8_t { direction_1, direction_2 };

    enum rotary_state : uint8_t {
        rotary_state_0 = 0x00,
        rotary_state_1 = 0x01,
        rotary_state_2 = 0x02,
        rotary_state_3 = 0x03,
        state_current_mask = 0x03,
        state_direction_1 = 1 << 2,
        state_direction_2 = 1 << 3,
        state_directions_mask = state_direction_1 | state_direction_2,
        state_direction_1_event = 1 << 6,
        state_direction_2_event = 1 << 7,
        state_directions_event_mask = state_direction_1_event | state_direction_2_event,
        state_clear_events_mask = (uint8_t)(~state_directions_event_mask)
    };

    class rotary_2phase_machine {
    public:
        static uint8_t handle_direction_1(uint8_t from, uint8_t to) {
            uint8_t result = from;
            uint8_t state = from & state_current_mask;
            if (state == rotary_state_2 && to == rotary_state_0) {
                result = to | state_direction_1_event;
            }
            if (state == rotary_state_1 && to == rotary_state_3) {
                result = to | state_direction_1_event;
            }
            return result;
        }

        static uint8_t handle_direction_2(uint8_t from, uint8_t to) {
            uint8_t result = from;
            uint8_t state = from & state_current_mask;
            if (state == rotary_state_2 && to == rotary_state_3) {
                result = to | state_direction_2_event;
            }
            if (state == rotary_state_1 && to == rotary_state_0) {
                result = to | state_direction_2_event;
            }
            return result;
        }

        static uint8_t handle(uint8_t from, uint8_t to) {
            uint8_t result = to;
            uint8_t direction = from & state_directions_mask;
            switch (direction) {
            case state_direction_1:
                return handle_direction_1(from, to);
            case state_direction_2:
                return handle_direction_2(from, to);
            default:
                switch (from) {
                case rotary_state_0:
                    if (to == rotary_state_1) {
                        result |= state_direction_1;
                    }
                    if (to == rotary_state_2) {
                        result |= state_direction_2;
                    }
                    break;
                case rotary_state_3:
                    if (to == rotary_state_1) {
                        result |= state_direction_2;
                    }
                    if (to == rotary_state_2) {
                        result |= state_direction_1;
                    }
                    break;
                default:
                    result = from;
                    break;
                }
                break;
            }

            return result;
        }
    };

    class rotary_2phase_machine_v2 {
    public:
        static uint8_t handle(uint8_t from, uint8_t to) {
            switch (from) {
            case b00:
                switch (to) {
                case b01:
                    return b01a;
                case b10:
                    return b10b;
                }
                break;
            case (b01a):
                switch (to) {
                case b01:
                    return b01a;
                case b11:
                    return (b11 | zoal::io::rotary_state::state_direction_1_event);
                default:
                    return to;
                }
            case (b01b):
                switch (to) {
                case b01:
                    return b01b;
                case b00:
                    return (b00 | zoal::io::rotary_state::state_direction_2_event);
                }
                break;
            case (b10a):
                switch (to) {
                case b10:
                    return b10a;
                case b00:
                    return (b00 | zoal::io::rotary_state::state_direction_1_event);
                }
                break;
            case (b10b):
                switch (to) {
                case b10:
                    return b10b;
                case b11:
                    return (b11 | zoal::io::rotary_state::state_direction_2_event);
                }
                break;
            case b11:
                switch (to) {
                case b01:
                    return b01b;
                case b10:
                    return b10a;
                }
                break;
            }
            return to;
        }
    private:
        static constexpr uint8_t dir_a = 0b0100;
        static constexpr uint8_t dir_b = 0b1000;
        static constexpr uint8_t b00 = 0b00;
        static constexpr uint8_t b01 = 0b01;
        static constexpr uint8_t b01a = b01 | dir_a;
        static constexpr uint8_t b01b = b01 | dir_b;
        static constexpr uint8_t b10 = 0b10;
        static constexpr uint8_t b10a = b10 | dir_a;
        static constexpr uint8_t b10b = b10 | dir_b;
        static constexpr uint8_t b11 = 0b11;
    };

    class rotary_4phase_machine {
    public:
        static uint8_t handle_direction_1(uint8_t from, uint8_t to) {
            uint8_t state = from & state_current_mask;
            if (state == rotary_state_1 && to == rotary_state_3) {
                return to | state_direction_1;
            }
            if (state == rotary_state_3 && to == rotary_state_0) {
                return to | state_direction_1_event;
            }
            return to == rotary_state_0 ? to : from;
        }

        static uint8_t handle_direction_2(uint8_t from, uint8_t to) {
            uint8_t state = from & state_current_mask;
            if (state == rotary_state_2 && to == rotary_state_3) {
                return to | state_direction_2;
            }
            if (state == rotary_state_3 && to == rotary_state_0) {
                return to | state_direction_2_event;
            }
            return to == rotary_state_0 ? to : from;
        }

        static uint8_t handle(uint8_t from, uint8_t to) {
            uint8_t result = to;
            uint8_t direction = from & state_directions_mask;
            switch (direction) {
            case state_direction_1:
                return handle_direction_1(from, to);
            case state_direction_2:
                return handle_direction_2(from, to);
            default:
                if (to == rotary_state_1) {
                    result |= state_direction_1;
                }
                if (to == rotary_state_2) {
                    result |= state_direction_2;
                }
                break;
            }

            return result;
        }
    };

    template<class A, class B, class StateMachine = rotary_4phase_machine, bool ActiveLow = false>
    class rotary_encoder {
    public:
        template<class Callback>
        void handle(Callback callback) {
            using namespace zoal::gpio;
            uint8_t to = A::read() << 1 | B::read();
            if (ActiveLow) {
                to = to ^ 0b11;
            }
            uint8_t state = StateMachine::handle(state_, to);
            uint8_t event = state & state_directions_event_mask;
            switch (event) {
            case state_direction_1_event:
                callback(rotary_event::direction_1);
                break;
            case state_direction_2_event:
                callback(rotary_event::direction_2);
                break;
            default:
                break;
            }

            state_ = (rotary_state)(state & state_clear_events_mask);
        }

        template<class T, class M>
        void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, rotary_event>(obj, m));
        }

        rotary_state state_{rotary_state::rotary_state_0};
    };
}}

#endif

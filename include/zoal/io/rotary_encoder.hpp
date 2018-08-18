#ifndef ZOAL_IO_ROTARY_ENCODER_HPP
#define ZOAL_IO_ROTARY_ENCODER_HPP

#include <stdint.h>
#include "../gpio/pin_mode.hpp"
#include "../utils/method_invoker.hpp"

namespace zoal { namespace io {
    enum class rotary_event : uint8_t {
        direction_1,
        direction_2
    };

    enum rotary_state : uint8_t {
        rotary_state_0 = 0x00,
        rotary_state_1 = 0x01,
        rotary_state_2 = 0x02,
        rotary_state_3 = 0x03,
        state_current_mask = 0x03,
        state_clear_events_mask = 0x0F,
        state_direction_1 = 1 << 2,
        state_direction_2 = 1 << 3,
        state_directions_mask = state_direction_1 | state_direction_2,
        state_direction_1_event = 1 << 4,
        state_direction_2_event = 1 << 5,
        state_directions_event_mask = state_direction_1_event | state_direction_2_event
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

    template<class A, class B, class StateMachine = rotary_4phase_machine, zoal::gpio::pin_mode PinMode = zoal::gpio::pin_mode::input_pull_up>
    class rotary_encoder {
        rotary_state rotaryState;
    public:
        void begin() {
            A::template mode<PinMode>();
            B::template mode<PinMode>();
        }

        template<class Callback>
        void handle(Callback callback) {
            using namespace zoal::gpio;
            uint8_t va = PinMode == pin_mode::input_pull_up ? A::read() ^ 1 : A::read();
            uint8_t vb = PinMode == pin_mode::input_pull_up ? B::read() ^ 1 : B::read();
            uint8_t to = va << 1 | vb;
            uint8_t state = StateMachine::handle(rotaryState, to);
            uint8_t event = state & state_directions_event_mask;
            switch (event) {
                case state_direction_1_event:
                    callback(rotary_event::direction_1);
                    break;
                case state_direction_2_event:
                    callback(rotary_event::direction_2);
                    break;
            }

            rotaryState = (rotary_state) (state & state_clear_events_mask);
        }

        template<class T, class M>
        void handle(T *obj, M m) {
            handle(zoal::utils::method_invoker<T, rotary_event>(obj, m));
        }
    };
}}

#endif

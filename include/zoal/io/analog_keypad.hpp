#ifndef ZOAL_IO_ANALOG_KEYPAD_HPP
#define ZOAL_IO_ANALOG_KEYPAD_HPP

#include "button_state_machine.hpp"

#include <stdint.h>
#include <string.h> /* NOLINT */

namespace zoal { namespace io {
    template<class Tools, uint8_t Count, int Threshold = 10>
    class analog_keypad {
    public:
        static constexpr auto button_count = Count;

        using button_value_type = uint16_t;
        using counter = typename Tools::counter;
        using delay = typename Tools::delay;
        using logger = typename Tools::logger;
        using counter_value_type = typename counter::value_type;

        analog_keypad() = delete;

        analog_keypad(const analog_keypad &) = delete;

        template<class H>
        static void handle(H handler, int16_t value) {
            using namespace zoal::io;

            zoal::io::button_state_machine machine(5, 300);
            counter_value_type milliseconds = counter::now();
            counter_value_type dt = milliseconds - prev_time;

            uint8_t allEvents = 0;
            for (int i = 0; i < button_count; i++) {
                uint8_t dv = 0;
                int buttonValue = values[i];
                if (buttonValue - Threshold < value && value < buttonValue + Threshold) {
                    dv = 1;
                }

                uint8_t state = machine.handle_button(dt, states[i], dv);
                uint8_t events = state & button_state_trigger;
                states[i] = state & ~button_state_trigger;
                allEvents |= events;

                if (events == 0) {
                    continue;
                }

                if ((events & button_state_trigger_down) != 0) {
                    logger::trace() << "ButtonStateTriggerDown: " << i;
                    handler(i, button_event::down);
                }

                if ((events & button_state_trigger_press) != 0) {
                    logger::trace() << "ButtonStateTriggerPress: " << i;
                    handler(i, button_event::press);
                }

                if ((events & button_state_trigger_up) != 0) {
                    logger::trace() << "ButtonStateTriggerUp: " << i;
                    handler(i, button_event::up);
                }
            }

            if (allEvents != 0) {
                prev_time = milliseconds;
            }
        }

        static button_value_type values[Count];

    protected:
        static counter_value_type prev_time;
        static uint8_t states[Count];
    };

    template<class Tools, uint8_t Count, int Threshold>
    typename analog_keypad<Tools, Count, Threshold>::button_value_type
        analog_keypad<Tools, Count, Threshold>::values[Count];

    template<class Tools, uint8_t Count, int Threshold>
    typename analog_keypad<Tools, Count, Threshold>::counter_value_type
        analog_keypad<Tools, Count, Threshold>::prev_time = 0;

    template<class Tools, uint8_t Count, int Threshold>
    uint8_t analog_keypad<Tools, Count, Threshold>::states[Count] = {0};
}}

#endif

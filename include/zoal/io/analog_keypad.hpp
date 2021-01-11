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

            counter_value_type current_time = counter::now();
            for (int i = 0; i < button_count; i++) {
                uint8_t dv = 0;
                int buttonValue = values[i];
                if (buttonValue - Threshold < value && value < buttonValue + Threshold) {
                    dv = 1;
                }

                auto dt = current_time - prev_time[i];
                auto switched = machines[i].handle(dv, dt, 5, 250);
                if (switched) {
                    prev_time[i] = current_time;
                }

                machines[i].invoke_callback(handler, i);
            }
        }

        static button_value_type values[Count];

    protected:
        static counter_value_type prev_time[Count];
        static zoal::io::button_state_machine machines[Count];
    };

    template<class Tools, uint8_t Count, int Threshold>
    typename analog_keypad<Tools, Count, Threshold>::button_value_type
        analog_keypad<Tools, Count, Threshold>::values[Count];

    template<class Tools, uint8_t Count, int Threshold>
    typename analog_keypad<Tools, Count, Threshold>::counter_value_type analog_keypad<Tools, Count, Threshold>::prev_time[Count] = {0};
}}

#endif

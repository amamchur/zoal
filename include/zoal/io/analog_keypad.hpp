#ifndef ZOAL_IO_ANALOG_KEYPAD_HPP
#define ZOAL_IO_ANALOG_KEYPAD_HPP

#include "button_state_machine.hpp"

#include <stdint.h>

namespace zoal { namespace io {
    template<class TimeType, uint8_t Count, int Threshold = 50>
    class analog_keypad {
    public:
        static constexpr auto button_count = Count;

        using button_value_type = uint16_t;
        using counter_value_type = TimeType;

        template<class H, class ...Args>
        void handle(H handler, TimeType current_time, int16_t value, Args... args) {
            using namespace zoal::io;

            for (int i = 0; i < button_count; i++) {
                uint8_t dv = 0;
                int buttonValue = adc_values[i];
                if (buttonValue - Threshold < value && value < buttonValue + Threshold) {
                    dv = 1;
                }

                counter_value_type dt = current_time - prev_time[i];
                auto switched = machines[i].handle(dv, dt, (counter_value_type)5, (counter_value_type)500, (counter_value_type)30);
                if (switched) {
                    prev_time[i] = current_time;
                }

                machines[i].invoke_callback(handler, i, args...);
            }
        }

        button_value_type adc_values[Count]{0};
        counter_value_type prev_time[Count]{0};
        zoal::io::button_state_machine machines[Count];
    };
}}

#endif

#ifndef ZOAL_STM32F103C8_INPUT_PROCESSOR_HPP
#define ZOAL_STM32F103C8_INPUT_PROCESSOR_HPP

#include "./board.hpp"
#include "./hardware.hpp"

#include <zoal/freertos/task.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>

using user_button_type = zoal::io::button<TickType_t, mcu::pa_00, zoal::io::active_high_button>;

[[noreturn]] void zoal_input_processor(void *);

#endif

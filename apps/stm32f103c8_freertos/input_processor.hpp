#ifndef ZOAL_STM32F103C8_INPUT_PROCESSOR_HPP
#define ZOAL_STM32F103C8_INPUT_PROCESSOR_HPP

#include "hardware.hpp"
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/freertos/task.hpp>

[[noreturn]] void zoal_input_processor(void *);

#endif

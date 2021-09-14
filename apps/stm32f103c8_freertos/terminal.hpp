#ifndef ZOAL_STM32F103C8_TERMINAL_HPP
#define ZOAL_STM32F103C8_TERMINAL_HPP

#include "../misc/terminal_input.hpp"

#include <zoal/mem/reserve_mem.hpp>

constexpr size_t input_string_length = 64;

extern zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;

void init_terminal();

#endif

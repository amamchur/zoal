#ifndef ZOAL_STM32F103C8_TERMINAL_HPP
#define ZOAL_STM32F103C8_TERMINAL_HPP

#include "../misc/terminal_input.hpp"

#include <FreeRTOS.h>
#include <task.h>
#include <zoal/freertos/task.hpp>
#include <zoal/mem/reserve_mem.hpp>

constexpr size_t input_string_length = 256;

extern zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
extern char command_history[input_string_length];

[[noreturn]] void zoal_terminal_rx_task(void *);

#endif

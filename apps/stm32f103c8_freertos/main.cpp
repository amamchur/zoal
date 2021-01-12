#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"
#include "board.hpp"
#include "command_processor.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "terminal.hpp"

#include <zoal/freertos/task.hpp>

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
zoal::mem::reserve_mem<task_type, 256, StackType_t> input_task(zoal_input_processor, "input");

extern "C" [[noreturn]] __unused void zoal_default_thread(void *) {
    static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.handle_v100(&handle_v100);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    tx_stream << zoal_ascii_logo << help_message;

    terminal.sync();

    for (;;) {
        uint8_t rx_buffer[8];
        auto size = rx_stream_buffer.receive(rx_buffer, sizeof(rx_buffer));
        if (size > 0) {
            terminal.push(rx_buffer, size);
        }
    }
}

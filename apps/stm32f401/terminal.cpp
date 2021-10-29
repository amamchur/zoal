#include "./terminal.hpp"

#include "./command_machine.hpp"
#include "./constants.hpp"
#include "./hardware.hpp"

#include <zoal/parse/type_parser.hpp>

zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
static char command_history[input_string_length] = {0};

static void handle_v100(const zoal::misc::terminal_input *, zoal::misc::terminal_machine_event e) {
    switch (e) {
    case zoal::misc::terminal_machine_event::up_key:
        terminal.value(command_history);
        break;
    case zoal::misc::terminal_machine_event::down_key:
        terminal.value("");
        break;
    default:
        break;
    }
}

static void command_callback(zoal::misc::command_machine *, app_cmd cmd, int argc, zoal::misc::cmd_arg *argv) {
    command_msg msg(cmd);

    switch (cmd) {
    case app_cmd::time_set:
        msg.date_time_value = zoal::parse::type_parser<zoal::data::date_time>::parse(argv->start);
        break;
    default:
        for (int i = 0; i < argc; i++) {
            msg.int_values[i] = (int)argv[i];
        }
        break;
    }

    command_queue.push(msg);
}

static void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    scoped_lock lock(tx_stream_mutex);
    tx_stream << "\r\n";

    if (s < e) {
        strcpy(command_history, s);

        zoal::misc::command_machine cm;
        cm.callback(command_callback);
        cm.run_machine(s, e, e);
    }

    terminal.sync();
}

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_01_tx_transport::send_data(s, e - s);
}

void init_terminal() {
    scoped_lock lock(tx_stream_mutex);

    static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.handle_v100(&handle_v100);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    tx_stream << zoal_ascii_logo << help_message;
    terminal.sync();
}

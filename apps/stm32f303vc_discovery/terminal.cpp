#include "./terminal.hpp"

#include "../misc/cmd_line_parser.hpp"
#include "./command_machine.hpp"
#include "./command_queue.hpp"
#include "./constants.hpp"
#include "./hardware.hpp"

using command_line_parser = zoal::misc::command_line_parser;
zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
char command_history[input_string_length] = {0};

class base_command {
public:
    base_command(const char *name, const char *usage, app_cmd cmd, base_command *next)
        : name_(name)
        , usage_(usage)
        , cmd_(cmd)
        , next_(next) {}

    template<class T>
    base_command(const char *name, const char *usage, app_cmd cmd, T t, base_command *next)
        : name_(name)
        , usage_(usage)
        , cmd_(cmd)
        , parse(t)
        , next_(next) {}

    const char *name_;
    const char *usage_;
    app_cmd cmd_;
    void (*parse)(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e){nullptr};
    base_command *next_;
};

static void take_task_name(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    command_msg msg(app_cmd::task_info);

    auto end = msg.str_value + sizeof(msg.str_value) - 1;
    auto src = ts;
    auto dst = msg.str_value;
    while (src != te && dst != end) *dst++ = *src++;
    *dst = 0;
    command_queue.push(msg);
}

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
    case app_cmd::task_info:
        argv->strcpy(msg.str_value);
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
        zoal::misc::command_machine cm;
        cm.callback(command_callback);
        cm.run_machine(s, e, e);
    }

    terminal.sync();
}

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx.send_data(s, e - s);
}

[[noreturn]] void zoal_terminal_rx_task(void *) {
    {
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

    for (;;) {
        uint8_t rx_buffer[8];
        auto size = usart_tx.rxs.receive(rx_buffer, sizeof(rx_buffer));
        if (size == 0) {
            continue;
        }

        terminal.push_and_scan(rx_buffer, size);
    }
}

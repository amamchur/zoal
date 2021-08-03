#include "./terminal.hpp"

#include "../misc/cmd_line_parser.hpp"
#include "./command_queue.hpp"
#include "./constants.hpp"
#include "./hardware.hpp"
#include "./types.hpp"

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
    command_msg msg(app_cmd_task_info);

    auto end = msg.task_name + sizeof(msg.task_name) - 1;
    auto src = ts;
    auto dst = msg.task_name;
    while (src != te && dst != end) *dst++ = *src++;
    *dst = 0;
    command_queue.push(msg);
}

base_command ticks_command("ticks", "\t\tprint current ticks", app_cmd_ticks, nullptr);
base_command task_info_command("task-info", "\tprint task info", app_cmd_none, take_task_name, &ticks_command);
base_command *root_cmd = &task_info_command;

template<class T>
bool cmp_str_token(T s1, const char *ss, const char *se) {
    while (*s1 && ss < se) {
        if (*s1 != *ss) {
            return false;
        }
        ++s1;
        ++ss;
    }
    return !*s1 && ss == se;
}

static uint8_t parse_time_component(const char *ptr) {
    return (ptr[0] - '0') * 10 + (ptr[1] - '0');
}

static void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    for (base_command *cmd = root_cmd; cmd != nullptr; cmd = cmd->next_) {
        bool result = cmp_str_token(cmd->name_, p->token_start(), p->token_end());
        if (result) {
            if (cmd->parse) {
                p->callback(cmd->parse);
            } else {
                command_queue.push(command_msg(cmd->cmd_));
            }
            return;
        }
    }

    command_queue.push(command_msg(app_cmd_not_found));
}

static void handle_v100(const zoal::misc::terminal_input *, zoal::misc::terminal_machine_event e) {
    switch (e) {
    case zoal::misc::terminal_machine_event::ctrl_up_key:
    case zoal::misc::terminal_machine_event::up_key:
        terminal.value(command_history);
        break;
    case zoal::misc::terminal_machine_event::ctrl_down_key:
    case zoal::misc::terminal_machine_event::down_key:
        terminal.value("");
        break;
    default:
        break;
    }
}

static void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    if (s == e) {
        return;
    }

    auto src = s;
    auto dst = command_history;
    while (src != e) *dst++ = *src++;
    *dst = 0;

    command_line_parser parser(nullptr, 0);
    parser.callback(cmd_select_callback);
    parser.scan(s, e, e);
}

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    tx_target.send_data(s, e - s);
}

[[noreturn]] void zoal_terminal_rx_task(void *) {
    {
        __unused usart_debug_tx_transport::scoped_lock lock(tx_target.mutex);

        static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
        terminal.vt100_feedback(&vt100_callback);
        terminal.input_callback(&input_callback);
        terminal.handle_v100(&handle_v100);
        terminal.greeting(terminal_greeting);
        terminal.clear();

        tx_stream << zoal_ascii_logo << help_message;

        base_command *cmd = root_cmd;
        while (cmd != nullptr) {
            tx_stream << "\t" << cmd->name_ << cmd->usage_ << "\r\n";
            cmd = cmd->next_;
        }

        terminal.sync();
    }

    for (;;) {
        uint8_t rx_buffer[8];
        auto size = tx_target.rxs.receive(rx_buffer, sizeof(rx_buffer));
        if (size == 0) {
            continue;
        }

        terminal.push(rx_buffer, size);
    }
}

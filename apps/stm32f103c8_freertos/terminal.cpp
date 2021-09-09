#include "./terminal.hpp"

#include "../misc/cmd_line_parser.hpp"
#include "../misc/type_detector.hpp"
#include "./command_queue.hpp"
#include "./constants.hpp"
#include "./hardware.hpp"

#include <zoal/parse/type_parser.hpp>

using command_line_parser = zoal::misc::command_line_parser;
zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
char command_history[input_string_length] = {0};

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

static void parser_time(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    zoal::misc::type_detector_v2 td;
    auto pos = td.detect(p->token_start(), p->token_end(), p->token_end());
    if (td.type() != zoal::misc::value_type::date_time) {
        tx_stream << "\r\n" << "Invalid date-time format" << "\r\n";
        return;
    }

    command_msg msg;
    msg.date_time = zoal::parse::type_parser<zoal::data::date_time>::parse(pos);
    msg.command = app_cmd_time_set;
    command_queue.push(msg);
}

static void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    command_msg msg;

    if (cmp_str_token("on", ts, te)) {
        msg.command = app_cmd_led_on;
    } else if (cmp_str_token("off", ts, te)) {
        msg.command = app_cmd_led_off;
    } else if (cmp_str_token("ticks", ts, te)) {
        msg.command = app_cmd_ticks;
    } else if (cmp_str_token("heap", ts, te)) {
        msg.command = app_cmd_print_heap_size;
    } else if (cmp_str_token("help", ts, te)) {
        msg.command = app_cmd_help;
    } else if (cmp_str_token("time", ts, te)) {
        msg.command = app_cmd_time_print;
    } else if (cmp_str_token("set-time", ts, te)) {
        p->callback(&parser_time);
        return;
    } else if (cmp_str_token("task-info", ts, te)) {
        p->callback(&take_task_name);
        return;
    } else if (cmp_str_token("scan-i2c", ts, te)) {
        msg.command = app_cmd_scan_i2c;
    } else if (cmp_str_token("read-eeprom", ts, te)) {
        msg.command = app_cmd_read_eeprom;
    } else {
        msg.command = app_cmd_not_found;
    }

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
    parser.exec_machine(s, e, e);
}

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_01_tx_transport::send_data(s, e - s);
}

void init_terminal() {
    static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.handle_v100(&handle_v100);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    tx_stream << zoal_ascii_logo << help_message;
    terminal.sync();
}

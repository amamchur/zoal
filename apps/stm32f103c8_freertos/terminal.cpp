#include "./terminal.hpp"

#include "../misc/cmd_line_parser.hpp"
#include "../misc/type_detector.hpp"
#include "./command_queue.hpp"
#include "./hardware.hpp"

#include <FreeRTOS.h>
#include <task.h>

using command_line_parser = zoal::misc::command_line_parser;
zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
char command_history[input_string_length] = {0};
TaskStatus_t taskStatus;

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
    char name[16] = {0};
    auto end = name + sizeof(name) - 1;
    auto src = ts;
    auto dst = name;
    while (src != te && dst != end) *dst++ = *src++;
    *dst = 0;

    TaskHandle_t xTask = xTaskGetHandle(name);
    if (xTask) {
        vTaskGetInfo(xTask, &taskStatus, pdTRUE, eInvalid);
        command_queue.push(command_msg(app_cmd_task_info));
    } else {
        command_queue.push(command_msg(app_cmd_not_found));
    }
}

static uint8_t parse_time_component(const char *ptr) {
    return (ptr[0] - '0') * 10 + (ptr[1] - '0');
}

static void type_callback(zoal::misc::type_detector_machine *pm, zoal::misc::value_type t) {
    pm->callback(&zoal::misc::type_detector_machine::empty_callback);
    if (t != zoal::misc::value_type::time) {
        command_queue.push(command_msg(app_cmd_not_found));
        return;
    }

    auto ptr = pm->token_start();
    command_msg msg(app_cmd_time_set);
    msg.update_time.time.Hours = parse_time_component(ptr);
    msg.update_time.time.Minutes = parse_time_component(ptr + 3);
    msg.update_time.time.Seconds = parse_time_component(ptr + 6);
    command_queue.push(msg);
}

static void parser_time(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        command_queue.push(command_msg(app_cmd_time_print));
        return;
    }

    zoal::misc::type_detector detector(nullptr, 0);
    detector.callback(&type_callback);
    detector.scan(p->token_start(), p->token_end());
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
    } else {
        msg.command = app_cmd_not_found;
    }
    command_queue.push(msg);
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
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

void handle_v100(const zoal::misc::terminal_input *, zoal::misc::terminal_machine_event e) {
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

void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_01_tx_transport::send_data(s, e - s);
}

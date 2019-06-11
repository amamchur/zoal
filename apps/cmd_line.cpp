#include "cmd_line_parser.hpp"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

enum class cmd_state {
    start,

    pin_control,
    pin_number,

    finished,
    error
};

cmd_state state = cmd_state::start;

void perform_transition_eq(cmd_state from, cmd_state to, const char *str, const char *s, const char *e) {
    if (state != from) {
        return;
    }

    auto *p = str;
    while (*p && (s != e)) {
        if (*p != *s) {
            return;
        }
        p++;
        s++;
    }

    if (*p == '\0' && s == e) {
        state = to;
    }
}

void callback(cmd_line_parser *p, event event) {
    switch (event) {
    case event::command_msg:
    case event::command_on:
    case event::command_off: {
        auto s = p->token_start();
        auto e = p->token_end();
        std::string str(s, e);
        std::cout << "command: " << str << std::endl;
        break;
    }
    case event::token: {
        auto s = p->token_start();
        auto e = p->token_end();
        perform_transition_eq(cmd_state::start, cmd_state::pin_control, "pin", s, e);

        std::string str(p->token_start(), p->token_end());
        std::cout << "token: " << str << std::endl;
        break;
    }
    case event::line_end:
        std::cout << "line_end" << std::endl;
        break;
    }
}

int main(int argc, char **argv) {
    char msg[] = R"(msg 12312 )"
                 "\r\n";
    auto p = msg;

    cmd_line_parser clp;
    clp.callback(&callback);
    while (*p) {
        clp.push(*p++);
    }

    return 0;
}

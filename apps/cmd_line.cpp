#include "cmd_line_parser.hpp"

#include <iostream>
#include <string>

using cmd_parser = command_line_parser<16>;

void callback(void *parser, int event) {
    auto p = reinterpret_cast<cmd_parser *>(parser);
    auto cle = static_cast<command_line_event>(event);

    switch (cle) {
    case command_line_event::command_msg:
    case command_line_event::command_on:
    case command_line_event::command_off:
    case command_line_event::token: {
        std::string str(p->token_start(), p->token_end());
        std::cout << "token: " << str << std::endl;
        break;
    }
    case command_line_event::line_end:
        std::cout << "line_end" << std::endl;
        break;
    }
}

int main(int argc, char **argv) {
    char msg[] = R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 R"(msg 123456789 on off 15962 7894654 1257896454 1324186789 531321 78123456789 123456789 123456789 123456789)"
                 "\r\n";
    cmd_parser clp;
    clp.callback(&callback);
    clp.push(msg, sizeof(msg) - 1);

//    int i = 0;
//    while (*p) {
//        std::cout << i << std::endl;
//        clp.push_char(*p++);
//        i++;
//    }

    return 0;
}

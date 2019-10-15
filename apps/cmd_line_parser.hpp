#ifndef CMD_LINE_PARSER_HPP
#define CMD_LINE_PARSER_HPP

#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <zoal/parser/ragel_scanner.hpp>

enum class command_line_event {
    command_msg,
    command_on,
    command_off,
    token,
    line_end
};


class command_line_parse_machine : public zoal::parser::scanner_machine<> {
public:
    void init_machine();
    void quoted_param_found_action();

    const char *run_machine(const char *p, const char *pe);
};

template<size_t BufferSize>
class command_line_parser : public zoal::parser::ragel_scanner<command_line_parse_machine, BufferSize> {
};

#endif

#ifndef CMD_LINE_PARSER_HPP
#define CMD_LINE_PARSER_HPP

#include <stddef.h>
#include <stdint.h>
#include <zoal/parser/ragel_scanner.hpp>

namespace zoal { namespace misc {
    enum class command_line_event { token, line_end };

    class command_line_parse_machine : public zoal::parser::scanner_machine<void*, command_line_event> {
    public:
        void init_machine();
        int start_state() const;
        void quoted_param_found_action();

        const char *run_machine(const char *p, const char *pe);
    };

    template<size_t BufferSize>
    class command_line_parser : public zoal::parser::ragel_scanner<command_line_parse_machine, BufferSize> {};
}}
#endif

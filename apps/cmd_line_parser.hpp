#ifndef CMD_LINE_PARSER_HPP
#define CMD_LINE_PARSER_HPP

class cmd_line_parser {
public:
    cmd_line_parser() noexcept;

    void do_parse(char const *p, const char *pe);

private:
    void terminator_found_action();

    void regular_param_found_action();

    void quoted_param_found_action();

    int cs{0};
    int act{0};
    const char *ts{nullptr};
    const char *te{nullptr};
    const char *eof{nullptr};
};

#endif

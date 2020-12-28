#ifndef ZOAL_STEPPER_28BYJ_MACHINE_HPP
#define ZOAL_STEPPER_28BYJ_MACHINE_HPP

#include <zoal/parser/ragel_scanner.hpp>

enum class command_line_event {
    command_rotate,
    command_stop,
    number_token,
    string_token,
    line_end
};


class stepper_28byj_machine : public zoal::parser::scanner_machine<> {
public:
    void init_machine();

    int start_state() const;

    const char *run_machine(const char *p, const char *pe);
};

template<size_t BufferSize>
class stepper_28byj_parser : public zoal::parser::ragel_scanner<stepper_28byj_machine, BufferSize> {
};

#endif

#ifndef ZOAL_TERMINAL_MACHINE_HPP
#define ZOAL_TERMINAL_MACHINE_HPP

#include <zoal/parser/ragel_scanner.hpp>

enum class terminal_machine_event {
    ctrl_up_key,
    ctrl_down_key,
    ctrl_right_key,
    ctrl_left_key,
    up_key,
    down_key,
    right_key,
    left_key,
    home_key,
    delete_key,
    end_key,
    backspace_key,
    ascii,
    carriage_return
};

class terminal_machine : public zoal::parser::scanner_machine<void *, terminal_machine_event> {
public:
    void init_machine();

    int start_state() const;

    const char *run_machine(const char *p, const char *pe);
};

template<size_t BufferSize>
class terminal_parser : public zoal::parser::ragel_scanner<terminal_machine, BufferSize> {
};

#endif

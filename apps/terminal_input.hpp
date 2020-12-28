#ifndef ZOAL_TERMINAL_INPUT_HPP
#define ZOAL_TERMINAL_INPUT_HPP

#include "terminal_machine.hpp"

class terminal_input : public zoal::parser::ragel_scanner<terminal_machine, 8> {
public:
    using self_type = terminal_input;

    typedef void (*callback_fn)(self_type *t, const char *s);

    terminal_input(char *buffer, size_t size);

    const char *str_start() const;
    const char *str_end() const;
    void clear();
    void sync();
    void vt100_callback(callback_fn fn);
    void input_callback(callback_fn fn);
    void greeting(const char *g);
public:
    const char *greeting_{nullptr};
    char *buffer_;
    char *cursor_;
    char *end_;
    size_t size_;
    callback_fn vt100_callback_{&empty_callback};
    callback_fn input_callback_{&empty_callback};

    void move_cursor(int dx);
    void insert_char(char ch);
    void do_backspace();
    void do_delete();
    void new_line();

    static void scanner_callback(void *parser, terminal_machine_event event);
    static void empty_callback(self_type *t, const char *s);
    void process_event(terminal_machine_event e);
    void move_to_word_end();
    void move_to_word_start();
    void normalize_cursor();
};

#endif

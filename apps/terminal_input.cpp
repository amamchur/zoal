
#include "terminal_input.hpp"

void terminal_input::empty_callback(self_type *t, const char *s) {
}

void terminal_input::scanner_callback(void *parser, terminal_machine_event event) {
    auto *me = reinterpret_cast<terminal_input *>(parser);
    me->process_event(event);
}

terminal_input::terminal_input(char *buffer, size_t size) {
    this->buffer_ = buffer;
    this->size_ = size;
    this->cursor_ = buffer_;
    this->end_ = buffer_;
    this->callback(&scanner_callback);
}

void terminal_input::move_cursor(int dx) {
    cursor_ += dx;
    normalize_cursor();
    sync();
}

void terminal_input::normalize_cursor() {
    auto l = buffer_;
    auto r = buffer_ == end_ ? end_ + 1 : end_;

    if (cursor_ < l) {
        cursor_ = l;
    }

    if (cursor_ > r) {
        cursor_ = r;
    }
}
void terminal_input::process_event(terminal_machine_event e) {
    switch (e) {
    case terminal_machine_event::ctrl_right_key:
        move_to_word_end();
        break;
    case terminal_machine_event::ctrl_left_key:
        move_to_word_start();
        break;
    case terminal_machine_event::right_key:
        move_cursor(1);
        break;
    case terminal_machine_event::left_key:
        move_cursor(-1);
        break;
    case terminal_machine_event::home_key:
        move_cursor(-size_);
        break;
    case terminal_machine_event::delete_key:
        do_delete();
        break;
    case terminal_machine_event::end_key:
        move_cursor(size_);
        break;
    case terminal_machine_event::backspace_key:
        do_backspace();
        break;
    case terminal_machine_event::ascii:
        insert_char(*token_start());
        break;
    case terminal_machine_event::carriage_return:
        *end_ = 0;
        input_callback_(this, buffer_);
        this->cursor_ = buffer_;
        this->end_ = buffer_;
        new_line();
        sync();
        break;
    default:
        break;
    }
}

void terminal_input::insert_char(char ch) {
    auto l = buffer_ + size_ - 1;
    if (end_ >= l) {
        return;
    }

    for (auto c = end_; c > cursor_; c--) {
        *c = *(c - 1);
    }

    *cursor_ = ch;
    cursor_++;
    end_++;
    sync();
}

void terminal_input::do_backspace() {
    if (cursor_ <= buffer_) {
        return;
    }

    for (auto c = cursor_; c < end_; c++) {
        *(c - 1) = *c;
    }

    end_--;
    cursor_--;
    sync();
}

void terminal_input::do_delete() {
    if (cursor_ >= end_) {
        return;
    }

    for (auto c = cursor_; c < end_; c++) {
        *c = *(c + 1);
    }

    end_--;
    sync();
}

const char *terminal_input::str_start() const {
    return this->buffer_;
}

const char *terminal_input::str_end() const {
    return end_;
}

void terminal_input::clear() {
    this->cursor_ = buffer_;
    this->end_ = buffer_;

    char cmd[] = "\033c";
    vt100_callback_(this, cmd);
}

void terminal_input::vt100_callback(callback_fn fn) {
    this->vt100_callback_ = fn;
}

void terminal_input::input_callback(callback_fn fn) {
    this->input_callback_ = fn;
}

void terminal_input::sync() {
    char clear_line[] = "\r\033[2K";
    vt100_callback_(this, clear_line);

    if (greeting_) {
        vt100_callback_(this, greeting_);
    }

    *end_ = 0;
    vt100_callback_(this, buffer_);

    char move_cur[] = "\033[000D";
    auto pos = end_ - cursor_;
    if (pos <= 0) {
        return;
    }

    char *p = move_cur + 4;
    while (pos > 0) {
        *p = (char)('0' + (pos % 10));
        pos /= 10;
        p--;
    }
    vt100_callback_(this, move_cur);
}

void terminal_input::greeting(const char *g) {
    greeting_ = g;
}

void terminal_input::new_line() {
    char new_line[] = "\r\n";
    vt100_callback_(this, new_line);
}

void terminal_input::move_to_word_end() {
    cursor_++;
    auto r = buffer_ == end_ ? end_ + 1 : end_;
    while (cursor_ < r && *cursor_ != ' ') cursor_++;
    normalize_cursor();
    sync();
}

void terminal_input::move_to_word_start() {
    cursor_--;
    while (cursor_ > buffer_ && *(cursor_ - 1) != ' ') cursor_--;
    normalize_cursor();
    sync();
}

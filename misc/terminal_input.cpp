
#include "terminal_input.hpp"

namespace zoal { namespace misc {

    void terminal_input::empty_callback(const self_type *, const char *, const char *) {}

    void terminal_input::scanner_callback(void *parser, terminal_machine_event event) {
        auto *me = reinterpret_cast<terminal_input *>(parser);
        me->process_event(event);
    }

    terminal_input::terminal_input() = default;

    terminal_input::terminal_input(char *buffer, size_t size) noexcept {
        this->init_buffer(buffer, size);
    }

    void terminal_input::init_buffer(char *buffer, size_t size) {
        this->buffer_ = buffer;
        this->size_ = size;
        this->cursor_ = buffer_;
        this->end_ = buffer_;
        this->callback(&scanner_callback);
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
            cursor(cursor_ + 1);
            ;
            break;
        case terminal_machine_event::left_key:
            cursor(cursor_ - 1);
            break;
        case terminal_machine_event::home_key:
            cursor(cursor_ - size_);
            break;
        case terminal_machine_event::delete_key:
            do_delete();
            break;
        case terminal_machine_event::end_key:
            cursor(cursor_ + size_);
            break;
        case terminal_machine_event::backspace_key:
            do_backspace();
            break;
        case terminal_machine_event::ascii:
            insert_char(*token_start());
            break;
        case terminal_machine_event::carriage_return:
            *end_ = 0;
            input_callback_(this, buffer_, end_);
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

        send_cstr("\033c");
    }

    void terminal_input::vt100_callback(callback_fn fn) {
        this->vt100_callback_ = fn;
    }

    void terminal_input::input_callback(callback_fn fn) {
        this->input_callback_ = fn;
    }

    void terminal_input::sync() const {
        send_cstr("\r\033[2K");

        if (greeting_) {
            send_cstr(greeting_);
        }

        *end_ = 0;
        vt100_callback_(this, buffer_, end_);

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
        send_cstr(move_cur);
    }

    void terminal_input::greeting(const char *g) {
        greeting_ = g;
    }

    void terminal_input::new_line() const {
        send_cstr("\r\n");
    }

    void terminal_input::move_to_word_end() {
        auto c = cursor_ + 1;
        auto r = buffer_ == end_ ? end_ + 1 : end_;
        while (c < r && *c != ' ') c++;
        cursor(c);
    }

    void terminal_input::move_to_word_start() {
        auto c = cursor_ - 1;
        while (c > buffer_ && *(c - 1) != ' ') c--;
        cursor(c);
    }

    void terminal_input::send_cstr(const char *str) const {
        auto e = str;
        while (*e++)
            ;
        vt100_callback_(this, str, e);
    }

    char *terminal_input::normalize_cursor(char *c) const {
        auto l = buffer_;
        auto r = buffer_ == end_ ? end_ + 1 : end_;

        if (c < l) {
            c = l;
        }

        if (c > r) {
            c = r;
        }

        return c;
    }

    void terminal_input::cursor(char *c) {
        c = normalize_cursor(c);
        if (c != cursor_) {
            cursor_ = c;
            sync();
        }
    }
}}

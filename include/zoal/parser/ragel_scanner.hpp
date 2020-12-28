#pragma once

#ifndef ZOAL_PARSER_RAGEL_SCANNER_HPP
#define ZOAL_PARSER_RAGEL_SCANNER_HPP

#include <stddef.h>

namespace zoal { namespace parser {
    template<class T = void *, class E = int>
    class scanner_callback {
    public:
        typedef void (*scanner_machine_callback)(T parser, E event);

        static void empty_callback(T p, E e) {}

        inline void callback(scanner_machine_callback cb) {
            this->handler_ = cb;
        }

        inline scanner_machine_callback callback() const {
            return this->handler_;
        }

    protected:
        scanner_machine_callback handler_{&empty_callback};
    };

    template<class T = void *, class E = int>
    class scanner_machine : public scanner_callback<T, E> {
    public:
        inline const char *token_start() const {
            return ts;
        }

        inline const char *token_end() const {
            return te;
        }

    protected:
        int cs{0};
        int act{0};
        const char *p{nullptr};
        const char *ts{nullptr};
        const char *te{nullptr};
        const char *eof{nullptr};
    };

    template<class Machine, size_t BufferSize>
    class ragel_scanner : public Machine {
    public:
        ragel_scanner() noexcept {
            reset();
        }

        void reset() {
            this->length_ = 0;
            this->p = buffer_;
            this->init_machine();
        }

        void push(const void *data, size_t size, const void *eof = nullptr) {
            auto dp = reinterpret_cast<const char *>(data);
            auto de = reinterpret_cast<const char *>(eof);
            do {
                this->eof = de ? this->buffer_ + (de - dp) : nullptr;

                while (size > 0 && length_ < BufferSize) {
                    this->buffer_[length_++] = *dp++;
                    size--;
                }

                auto be = this->buffer_ + length_;
                this->p = this->run_machine(this->p, be);

                if (this->ts > this->buffer_ || this->cs == this->start_state()) {
                    auto dst = this->buffer_;
                    auto ptr = this->ts ? this->ts : this->p;
                    auto src = ptr;
                    while (src != be) {
                        *dst++ = *src++;
                    }

                    this->p = this->buffer_ + (this->p - ptr);
                    this->te = this->buffer_ + (this->te - ptr);
                    this->ts = this->buffer_;
                    length_ = dst - this->buffer_;
                }
            } while (size > 0);
        }

        inline size_t length() const {
            return length_;
        }

    protected:
        size_t length_{0};
        char buffer_[BufferSize]{0};
    };
}}

#endif

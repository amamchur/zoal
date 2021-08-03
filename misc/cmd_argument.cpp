#include "cmd_argument.hpp"

namespace zoal { namespace misc {
    cmd_argument *cmd_argument::from_buffer(void *buffer) noexcept {
        auto v = reinterpret_cast<cmd_argument *>(buffer);
        v->length = 0;
        v->type = zoal::misc::value_type::string;
        return v;
    }

    void cmd_argument::string(const char *s, const char *e) {
        type = zoal::misc::value_type::string;
        auto ptr = reinterpret_cast<char *>(buffer);
        while (s != e) {
            *ptr++ = *s++;
        }
        *ptr++ = '\0';
        length = ptr - reinterpret_cast<char *>(buffer);
    }

    void cmd_argument::integer(const char *s, const char *e) {
        type = zoal::misc::value_type::integer;

        int value = 0;
        e--;
        while (e >= s && *e != '-') {
            value = value * 10 + (*e - '0');
            e--;
        }

        length = sizeof(int);
        *reinterpret_cast<int *>(buffer) = value * (*e == '-' ? -1 : 1);
    }

    cmd_argument *cmd_argument::prepare_next() {
        if (length == 0) {
            return this;
        }

        auto *ptr = reinterpret_cast<uint8_t *>(this);
        ptr += sizeof(cmd_argument) + length;
        auto v = reinterpret_cast<cmd_argument *>(ptr);
        v->length = 0;
        v->type = zoal::misc::value_type::string;
        return v;
    }

    cmd_argument *cmd_argument::next() {
        auto *ptr = reinterpret_cast<uint8_t *>(this);
        ptr += sizeof(cmd_argument) + length;
        auto v = reinterpret_cast<cmd_argument *>(ptr);
        return v->length == 0 ? nullptr : v;
    }
}}

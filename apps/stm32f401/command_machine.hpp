#ifndef ZOAL_COMMAND_MACHINE_HPP
#define ZOAL_COMMAND_MACHINE_HPP

#include "command_queue.hpp"

#include <zoal/parse/ragel_parser.hpp>

namespace zoal { namespace misc {
    class cmd_arg {
    public:
        const char *start{nullptr};
        const char *end{nullptr};

        explicit operator int() const {
            int result = 0, sign = 1;
            auto *p = start;
            if (*p == '-') {
                sign = -1;
                p++;
            }
            while (p < end) {
                result = result * 10 + *p++ - '0';
            }
            return sign * result;
        }

        void strcpy(char *dest) const {
            auto d = dest;
            auto s = start + 1;
            auto e = end - 1;
            while (s < e) {
                *d++ = *s++;
            }
            *d = '\0';
        }
    };

    class command_machine : public zoal::parse::ragel_machine<command_machine *, app_cmd, int, cmd_arg *> {
    public:
        command_machine() noexcept;

        void reset();
        static int start_state();
        const char *run_machine(const char *p, const char *pe, const char *eof);

    private:
        app_cmd command_{app_cmd::none};
        cmd_arg argv[1];
        int argc{0};
    };
}}

#endif

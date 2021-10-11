#ifndef ZOAL_COMMAND_MACHINE_HPP
#define ZOAL_COMMAND_MACHINE_HPP

#include <zoal/parse/ragel_parser.hpp>

namespace zoal { namespace misc {
    enum class command {
        //
        none,
        pwm,
        pwm_connect,
        pwm_disconnect
    };

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
    };

    class command_machine : public zoal::parse::ragel_machine<command_machine *, command, int, cmd_arg *> {
    public:
        command_machine() noexcept;

        void reset();
        static int start_state();
        const char *run_machine(const char *p, const char *pe, const char *eof);

    private:
        command command_{command::none};
        cmd_arg argv[3];
        int argc{0};
    };
}}

#endif

#ifndef ZOAL_COMMAND_MACHINE_HPP
#define ZOAL_TEST_MACHINE_HPP

#include <zoal/parse/ragel_parser.hpp>

namespace zoal { namespace misc {
    class test_argument {
    public:
        const char *start;
        const char *end;
    };

    class test_machine : public zoal::parse::ragel_machine<test_machine *, int, int, test_argument*> {
    public:
        void init_machine();
        static int start_state();
        const char *run_machine(const char *p, const char *pe, const char *eof);
    private:
        int command_{-1};
        test_argument argv[2];
        int argc{0};
    };

    class test_parser : public zoal::parse::ragel_parser<test_machine> {
    public:
        test_parser(void *buffer, size_t buffer_size) noexcept
            : ragel_parser(buffer, buffer_size) {}
    };
}}

#endif

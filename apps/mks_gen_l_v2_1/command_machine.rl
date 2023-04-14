%%{
	machine fsm_test;

	action finished {
	    this->handler_(this, this->command_, this->argc, this->argv);
	}

	action arg_start {
        argv[argc].start = p;
	}

    action arg_finished {
        argv[argc++].end = p;
    }

    hours =  ([0-1] . digit) | ('2' . [0-3]);
    min_sec = ([0-5] . digit);
    time = hours . ':' . min_sec . ':' min_sec;

    year = ('0') | ('1'..'9' . digit*);
    month = ('0' . digit) | ('1' . '0..2');
    day = ('3' . '0'..'1') | ('0'..'2' . digit);
    date = year . '.' . month . '.' day;

    date_time = (date . space . time) >arg_start %arg_finished;
    quoted_string = ('"' ((ascii - cntrl - space - '\\') | [ \t] | '\\'["tvfnr])+ '"') >arg_start %arg_finished;
	positive = ([1-9] digit*) >arg_start %arg_finished;
	integer = ('-'? digit+) >arg_start %arg_finished;

    cmd_ls = ('ls') %{ this->command_ = command::ls; };
    cmd_enc = ('enc') %{ this->command_ = command::enc; };
    cmd_clear = ('clear') %{ this->command_ = command::clear; };
    cmd_print = ('print' space+ quoted_string) %{ this->command_ = command::print; };
    cmd_print_at_row = ('print' space+ integer space+ quoted_string) %{ this->command_ = command::print_at_row; };
    cmd_print_at_row_column = ('print' space+ integer space+ integer space+ quoted_string) %{ this->command_ = command::print_at_row_column; };
    cmd_pwm = ('pwm' space+ integer) %{ this->command_ = command::pwm; };

    commands = (
        cmd_ls |
        cmd_enc |
        cmd_clear |
        cmd_print |
        cmd_print_at_row |
        cmd_print_at_row_column |
        cmd_pwm
    );

	main := (space* commands space*) %finished;
}%%

#include "command_machine.hpp"

namespace zoal { namespace misc {
    %% write data noerror nofinal noentry;

    command_machine::command_machine() noexcept {
        reset();
    }

    void command_machine::reset() {
        this->command_ = command::none;
        this->argc = 0;

        %% write init;
    }

    int command_machine::start_state() {
        return %%{ write start; }%%;
    }

    const char *command_machine::run_machine(const char *p, const char *pe, const char *eof) {
        %% write exec;
        return p;
    }
}}

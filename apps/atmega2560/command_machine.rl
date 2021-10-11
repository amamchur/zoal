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

    cmd_pwm_connect = ('pwm_connect') %{ this->command_ = command::pwm_connect; };
    cmd_pwm_disconnect = ('pwm_disconnect') %{ this->command_ = command::pwm_disconnect; };
    cmd_pwm = ('pwm' space+ integer) %{ this->command_ = command::pwm; };

    commands = (
        cmd_pwm_connect |
        cmd_pwm_disconnect |
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

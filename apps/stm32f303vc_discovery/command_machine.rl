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
    bit = [01] >arg_start %arg_finished;
    natural_number = ([1-9] digit+) >arg_start %arg_finished;
	int_number = ('-'? digit+) >arg_start %arg_finished;
	uint_number = ('0' | (digit+)) >arg_start %arg_finished;

    cmd_axis = 'axis' %{ this->command_ = app_cmd::axis; };
    cmd_help = 'help' %{ this->command_ = app_cmd::help; };
    cmd_i2c = 'i2c' %{ this->command_ = app_cmd::scan_i2c; };
    cmd_led = ('led' uint_number space+ bit) %{ this->command_ = app_cmd::led; };
    cmd_leds = ('leds' space+ bit) %{ this->command_ = app_cmd::leds; };
    cmd_ticks = 'ticks' %{ this->command_ = app_cmd::ticks; };

    commands = (
        cmd_axis |
        cmd_help |
        cmd_i2c |
        cmd_led |
        cmd_leds |
        cmd_ticks
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
        this->command_ = app_cmd::none;
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

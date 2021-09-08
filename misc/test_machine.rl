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

	positive = ([1-9] digit*) >arg_start %arg_finished;
	integer = ('-'? digit+) >arg_start %arg_finished;

    cmd_adc = 'adc' %{ this->command_ = 3; };
    cmd_calibrate = 'calibrate' %{ this->command_ = 2; };
    cmd_go = 'go' %{ this->command_ = 0; };
    cmd_i2c = 'i2c' %{ this->command_ = 6; };
    cmd_next = 'next' %{ this->command_ = 1; };
    cmd_play = ('play' space+ positive) %{ this->command_ = 7; };
    cmd_pump = 'pump' %{ this->command_ = 4; };
    cmd_step = ('step' space+ integer) %{ this->command_ = 8; };
    cmd_valve = 'valve' %{ this->command_ = 5; };

    commands = (
        cmd_adc |
        cmd_calibrate |
        cmd_go |
        cmd_i2c |
        cmd_next |
        cmd_play |
        cmd_pump |
        cmd_step |
        cmd_valve
    );

	main := (space* commands space*) %finished;
}%%

#include "test_machine.hpp"

namespace zoal { namespace misc {
    %% write data noerror nofinal noentry;

    void test_machine::init_machine() {
        this->command_ = -1;
        this->argc = 0;

        %% write init;
    }

    int test_machine::start_state() {
        return %%{ write start; }%%;
    }

    const char *test_machine::run_machine(const char *p, const char *pe, const char *eof) {
        %% write exec;
        return p;
    }
}}

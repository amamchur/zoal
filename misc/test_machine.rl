%%{
	machine fsm_name;

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

    go_cmd = 'go' %{ this->command_ = 0; };
    next_cmd = 'next' %{ this->command_ = 1; };
    calibrate_cmd = 'calibrate' %{ this->command_ = 2; };
    adc_cmd = 'adc' %{ this->command_ = 3; };
    pump_cmd = 'pump' %{ this->command_ = 4; };
    valve_cmd = 'valve' %{ this->command_ = 5; };
    i2c_cmd = 'i2c' %{ this->command_ = 6; };
    play_cmd = ('play' space+ positive) %{ this->command_ = 7; };
    step_cmd = ('step' space+ integer) %{ this->command_ = 8; };

    commands = (
        go_cmd |
        next_cmd |
        calibrate_cmd |
        adc_cmd |
        pump_cmd |
        i2c_cmd |
        i2c_cmd |
        step_cmd
    );

	main := (space* commands space*) %finished;
}%%

#include "test_machine.hpp"

namespace {
%% write data;
}

namespace zoal { namespace misc {
    void test_machine::init_machine() {
        this->command_ = -1;
        this->argc = 0;

        %% write init;
    }

    int test_machine::start_state() {
        return fsm_name_start;
    }

    const char *test_machine::run_machine(const char *p, const char *pe) {
        eof = pe;
        %% write exec;
        return p;
    }
}}

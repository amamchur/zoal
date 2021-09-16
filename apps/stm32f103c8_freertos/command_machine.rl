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
	signed_int = ('-'? digit+) >arg_start %arg_finished;
	unsigned_int = (digit+) >arg_start %arg_finished;

    cmd_eeprom = 'eeprom' %{ this->command_ = app_cmd::read_eeprom; };
    cmd_heap = 'heap' %{ this->command_ = app_cmd::print_heap_size; };
    cmd_help = 'help' %{ this->command_ = app_cmd::help; };
    cmd_i2c = 'i2c' %{ this->command_ = app_cmd::scan_i2c; };
    cmd_led = ('led' space+ signed_int) %{ this->command_ = app_cmd::led; };
    cmd_task_info = ('task-info' space+ quoted_string) %{ this->command_ = app_cmd::task_info; };
    cmd_ticks = 'ticks' %{ this->command_ = app_cmd::ticks; };
    cmd_time = 'time' %{ this->command_ = app_cmd::time_print; };
    cmd_time_set = ('time' space+ date_time) %{ this->command_ = app_cmd::time_set; };

    commands = (
        cmd_eeprom |
        cmd_heap |
        cmd_help |
        cmd_i2c |
        cmd_led |
        cmd_task_info |
        cmd_ticks |
        cmd_time |
        cmd_time_set
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

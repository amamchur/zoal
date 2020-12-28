#include "stepper_28byj_machine.hpp"

%%{
	machine fsm_name;

	main := |*
	    'rotate' => {
    	    handler_(this, (int)command_line_event::command_rotate);
        };
        'stop' => {
            handler_(this, (int)command_line_event::command_stop);
        };
        [\+\-]? digit+ =>{
            handler_(this, (int)command_line_event::number_token);
        };
        (ascii - space - ['"\\])+ => {
            handler_(this, (int)command_line_event::string_token);
        };
        32;
        '\n' => {
            handler_(this, (int)command_line_event::line_end);
        };

        any;
	*|;
}%%

namespace {
%% write data;
}

int stepper_28byj_machine::start_state() const {
    return fsm_name_start;
}

void stepper_28byj_machine::init_machine() {
    %% write init;
}

const char* stepper_28byj_machine::run_machine(const char *p, const char *pe) {
    %% write exec;
    return p;
}
#include "cmd_line_parser.hpp"

%%{
	machine fsm_name;

	action quoted_param_found {
		quoted_param_found_action();
	}

    action command_found {
        std::cout << "ASDFGHJKL" << std::endl;
    }

	quoted_param = '"' ((ascii - cntrl - space - '\\') | [ \t] | '\\'["tvfnr])+ '"';

	main := |*
	    'msg' => {
    	    handler_(this, (int)command_line_event::command_msg);
        };
        'on' => {
            handler_(this, (int)command_line_event::command_on);
        };
        'off' => {
            handler_(this, (int)command_line_event::command_off);
        };
        [\+\-]? digit+ =>{
            handler_(this, (int)command_line_event::token);;
        };
        quoted_param => quoted_param_found;
        (ascii - space - ['"\\])+ => {
            handler_(this, (int)command_line_event::token);
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

void command_line_parse_machine::quoted_param_found_action() {
    ts++;
    te--;

    auto dst = const_cast<char *>(ts);
    for (auto ptr = ts; ptr != te; ptr++) {
        if (*ptr != '\\') {
            *dst++ = *ptr;
            continue;
        }

        switch (*++ptr) {
            case 'n':
                *dst = '\n';
                break;
            case 'r':
                *dst = '\r';
                break;
            case 't':
                *dst = '\t';
                break;
            case 'v':
                *dst = '\v';
                break;
            case 'f':
                *dst = '\f';
                break;
            default:
                *dst = *ptr;
                break;
        }

        dst++;
    }

    te = dst;

    if (handler_) {
        handler_(this, (int)command_line_event::token);
    }
}

void command_line_parse_machine::init_machine() {
    %% write init;
}

const char* command_line_parse_machine::run_machine(const char *p, const char *pe) {
    %% write exec;
    return p;
}

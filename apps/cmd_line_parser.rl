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
    	    handler_(this, event::command_msg);
        };
        'on' => {
            handler_(this, event::command_on);
        };
        'off' => {
            handler_(this, event::command_off);
        };
        [\+\-]? digit+ =>{
            std::cout << "Number" << std::endl;
        };
        quoted_param => quoted_param_found;
        (ascii - space - ['"\\])+ => {
            handler_(this, event::token);
        };
        32;
        '\n' => {
            handler_(this, event::line_end);
        };

        any;
	*|;
}%%

namespace {
%% write data;
}

void cmd_line_parser::quoted_param_found_action() {
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
        handler_(this, event::token);
    }
}

void cmd_line_parser::init() {
    %% write init;
}

const char* cmd_line_parser::do_parse(const char *p, const char *pe) {
    %% write exec;
    return p;
}

void cmd_line_parser::empty_callback(cmd_line_parser *p, event e) {

}

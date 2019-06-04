#include "cmd_line_parser.hpp"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>

%%{
	machine fsm_name;
	action regular_param_found {
		regular_param_found_action();
	}

	action quoted_param_found {
		quoted_param_found_action();
	}

    action terminator_found {
        terminator_found_action();
    }

	quoted_param = '"' (digit | alpha | [ \t] | '\\'["tvfnr])+ '"';

	main := |*
	    quoted_param => quoted_param_found;
	    (ascii - space - '"')+ => regular_param_found;
        32*;
        [\r\n]+ => terminator_found;
	*|;
}%%

namespace {
%% write data;
}

void cmd_line_parser::terminator_found_action() {
    std::cout << "terminator_found_action: " << std::endl;
}

void cmd_line_parser::regular_param_found_action() {
    std::string str(ts, te);
    std::cout << "regular_param_found_action: " << str << std::endl;
}

void cmd_line_parser::quoted_param_found_action() {
    std::string str(ts, te);
    std::cout << "quoted_param_found_action: " << str << std::endl;
}

cmd_line_parser::cmd_line_parser() noexcept {
    %% write init;
}

void cmd_line_parser::do_parse(const char *p, const char *pe) {
    %% write exec;
}

#include "type_detector.hpp"

%%{
	machine fsm_name;
    hours =  ([0-1] . digit) | ('2' . [0-3]);
    min_sec = ([0-5] . digit);
    time = hours . ':' . min_sec . ':' min_sec;
	main := |*
    time => {
        handler_(this, value_type::time);
    };
    space;
    any => {
        handler_(this, value_type::unknown);
    };
	*|;
}%%

namespace {
%% write data;
}

namespace zoal { namespace misc {
    void type_detector_machine::init_machine() {
        %% write init;
    }

    const char *type_detector_machine::run_machine(const char *p, const char *pe) {
        %% write exec;
        return p;
    }

    int type_detector_machine::start_state() const {
        return fsm_name_start;
    }
}}

#include "type_detector.hpp"

%%{
	machine fsm_name;
    hours =  ([0-1] . digit) | ('2' . [0-3]);
    min_sec = ([0-5] . digit);
    time = hours . ':' . min_sec . ':' min_sec;

    year = ('0') | ('1'..'9' . digit*);
    month = ('0' . digit) | ('1' . '0..2');
    day = ('3' . '0'..'1') | ('0'..'2' . digit);
    date = year . '.' . month . '.' day;

    date_time = date . space . time;
    integer = '-'? . digit*;

    main := |*
    date_time => {
        handler_(this, value_type::date_time);
        fbreak;
    };
    date => {
        handler_(this, value_type::date);
        fbreak;
    };
    time => {
        handler_(this, value_type::time);
        fbreak;
    };
    integer => {
        handler_(this, value_type::integer);
        fbreak;
    };
    space;
    any+ => {
        handler_(this, value_type::string);
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

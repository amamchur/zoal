
/* #line 1 "./apps/cmd_line_parser.rl" */
#include "cmd_line_parser.hpp"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>


/* #line 30 "./apps/cmd_line_parser.rl" */


namespace {

/* #line 17 "./apps/cmd_line_parser.cpp" */
static const int fsm_name_start = 4;
static const int fsm_name_first_final = 4;
static const int fsm_name_error = 0;

static const int fsm_name_en_main = 4;


/* #line 34 "./apps/cmd_line_parser.rl" */
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
    
/* #line 44 "./apps/cmd_line_parser.cpp" */
	{
	cs = fsm_name_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 52 "./apps/cmd_line_parser.rl" */
}

void cmd_line_parser::do_parse(const char *p, const char *pe) {
    
/* #line 57 "./apps/cmd_line_parser.cpp" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr3:
/* #line 14 "./apps/cmd_line_parser.rl" */
	{te = p+1;{
		quoted_param_found_action();
	}}
	goto st4;
tr8:
/* #line 10 "./apps/cmd_line_parser.rl" */
	{te = p;p--;{
		regular_param_found_action();
	}}
	goto st4;
tr9:
/* #line 18 "./apps/cmd_line_parser.rl" */
	{te = p;p--;{
        terminator_found_action();
    }}
	goto st4;
tr10:
/* #line 27 "./apps/cmd_line_parser.rl" */
	{te = p;p--;}
	goto st4;
st4:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof4;
case 4:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 93 "./apps/cmd_line_parser.cpp" */
	switch( (*p) ) {
		case 10: goto st6;
		case 13: goto st6;
		case 32: goto st7;
		case 34: goto st1;
	}
	if ( (*p) > 8 ) {
		if ( 14 <= (*p) )
			goto st5;
	} else if ( (*p) >= 0 )
		goto st5;
	goto st0;
st0:
cs = 0;
	goto _out;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 33 )
		goto st5;
	if ( (*p) < 14 ) {
		if ( 0 <= (*p) && (*p) <= 8 )
			goto st5;
	} else if ( (*p) > 31 ) {
		if ( 35 <= (*p) )
			goto st5;
	} else
		goto st5;
	goto tr8;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 10: goto st6;
		case 13: goto st6;
	}
	goto tr9;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 32 )
		goto st7;
	goto tr10;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 9: goto st2;
		case 32: goto st2;
		case 92: goto st3;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st2;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st2;
	} else
		goto st2;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 9: goto st2;
		case 32: goto st2;
		case 34: goto tr3;
		case 92: goto st3;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st2;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st2;
	} else
		goto st2;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 34: goto st2;
		case 102: goto st2;
		case 110: goto st2;
		case 114: goto st2;
		case 116: goto st2;
		case 118: goto st2;
	}
	goto st0;
	}
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 5: goto tr8;
	case 6: goto tr9;
	case 7: goto tr10;
	}
	}

	_out: {}
	}

/* #line 56 "./apps/cmd_line_parser.rl" */
}

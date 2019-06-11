
/* #line 1 "./apps/cmd_line_parser.rl" */
#include "cmd_line_parser.hpp"


/* #line 40 "./apps/cmd_line_parser.rl" */


namespace {

/* #line 12 "./apps/cmd_line_parser.cpp" */
static const int fsm_name_start = 2;
static const int fsm_name_first_final = 2;
static const int fsm_name_error = -1;

static const int fsm_name_en_main = 2;


/* #line 44 "./apps/cmd_line_parser.rl" */
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
    
/* #line 67 "./apps/cmd_line_parser.cpp" */
	{
	cs = fsm_name_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 90 "./apps/cmd_line_parser.rl" */
}

const char* cmd_line_parser::do_parse(const char *p, const char *pe) {
    
/* #line 80 "./apps/cmd_line_parser.cpp" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 1:
	{{p = ((te))-1;}
    	    handler_(this, event::command_msg);
        }
	break;
	case 2:
	{{p = ((te))-1;}
            handler_(this, event::command_on);
        }
	break;
	case 3:
	{{p = ((te))-1;}
            handler_(this, event::command_off);
        }
	break;
	case 4:
	{{p = ((te))-1;}
            std::cout << "Number" << std::endl;
        }
	break;
	case 5:
	{{p = ((te))-1;}
		quoted_param_found_action();
	}
	break;
	case 6:
	{{p = ((te))-1;}
            handler_(this, event::token);
        }
	break;
	default:
	{{p = ((te))-1;}}
	break;
	}
	}
	goto st2;
tr4:
/* #line 38 "./apps/cmd_line_parser.rl" */
	{te = p+1;}
	goto st2;
tr6:
/* #line 34 "./apps/cmd_line_parser.rl" */
	{te = p+1;{
            handler_(this, event::line_end);
        }}
	goto st2;
tr7:
/* #line 33 "./apps/cmd_line_parser.rl" */
	{te = p+1;}
	goto st2;
tr13:
/* #line 38 "./apps/cmd_line_parser.rl" */
	{te = p;p--;}
	goto st2;
tr14:
/* #line 6 "./apps/cmd_line_parser.rl" */
	{te = p;p--;{
		quoted_param_found_action();
	}}
	goto st2;
tr15:
/* #line 30 "./apps/cmd_line_parser.rl" */
	{te = p;p--;{
            handler_(this, event::token);
        }}
	goto st2;
st2:
/* #line 1 "NONE" */
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 1 "NONE" */
	{ts = p;}
/* #line 163 "./apps/cmd_line_parser.cpp" */
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr7;
		case 34: goto tr8;
		case 43: goto tr9;
		case 45: goto tr9;
		case 109: goto st7;
		case 111: goto st9;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 8 ) {
			if ( 14 <= (*p) && (*p) <= 38 )
				goto tr5;
		} else if ( (*p) >= 0 )
			goto tr5;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 58 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr10;
		} else if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else
			goto tr5;
	} else
		goto tr5;
	goto tr4;
tr5:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 30 "./apps/cmd_line_parser.rl" */
	{act = 6;}
	goto st3;
tr17:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 17 "./apps/cmd_line_parser.rl" */
	{act = 1;}
	goto st3;
tr19:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 20 "./apps/cmd_line_parser.rl" */
	{act = 2;}
	goto st3;
tr20:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 23 "./apps/cmd_line_parser.rl" */
	{act = 3;}
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
/* #line 219 "./apps/cmd_line_parser.cpp" */
	if ( (*p) == 33 )
		goto tr5;
	if ( (*p) < 35 ) {
		if ( (*p) > 8 ) {
			if ( 14 <= (*p) && (*p) <= 31 )
				goto tr5;
		} else if ( (*p) >= 0 )
			goto tr5;
	} else if ( (*p) > 38 ) {
		if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else if ( (*p) >= 40 )
			goto tr5;
	} else
		goto tr5;
	goto tr0;
tr8:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 38 "./apps/cmd_line_parser.rl" */
	{act = 9;}
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
/* #line 247 "./apps/cmd_line_parser.cpp" */
	switch( (*p) ) {
		case 9: goto st0;
		case 92: goto st1;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st0;
	goto tr13;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
	switch( (*p) ) {
		case 9: goto st0;
		case 34: goto tr2;
		case 92: goto st1;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st0;
	goto tr0;
tr2:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 6 "./apps/cmd_line_parser.rl" */
	{act = 5;}
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
/* #line 277 "./apps/cmd_line_parser.cpp" */
	switch( (*p) ) {
		case 9: goto st0;
		case 34: goto tr2;
		case 92: goto st1;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto st0;
	goto tr14;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 34: goto st0;
		case 102: goto st0;
		case 110: goto st0;
		case 114: goto st0;
		case 116: goto st0;
		case 118: goto st0;
	}
	goto tr0;
tr9:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 30 "./apps/cmd_line_parser.rl" */
	{act = 6;}
	goto st6;
tr10:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 26 "./apps/cmd_line_parser.rl" */
	{act = 4;}
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 315 "./apps/cmd_line_parser.cpp" */
	if ( (*p) == 33 )
		goto tr5;
	if ( (*p) < 40 ) {
		if ( (*p) < 14 ) {
			if ( 0 <= (*p) && (*p) <= 8 )
				goto tr5;
		} else if ( (*p) > 31 ) {
			if ( 35 <= (*p) && (*p) <= 38 )
				goto tr5;
		} else
			goto tr5;
	} else if ( (*p) > 47 ) {
		if ( (*p) < 58 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr10;
		} else if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else
			goto tr5;
	} else
		goto tr5;
	goto tr0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 33: goto tr5;
		case 115: goto st8;
	}
	if ( (*p) < 35 ) {
		if ( (*p) > 8 ) {
			if ( 14 <= (*p) && (*p) <= 31 )
				goto tr5;
		} else if ( (*p) >= 0 )
			goto tr5;
	} else if ( (*p) > 38 ) {
		if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else if ( (*p) >= 40 )
			goto tr5;
	} else
		goto tr5;
	goto tr15;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 33: goto tr5;
		case 103: goto tr17;
	}
	if ( (*p) < 35 ) {
		if ( (*p) > 8 ) {
			if ( 14 <= (*p) && (*p) <= 31 )
				goto tr5;
		} else if ( (*p) >= 0 )
			goto tr5;
	} else if ( (*p) > 38 ) {
		if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else if ( (*p) >= 40 )
			goto tr5;
	} else
		goto tr5;
	goto tr15;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 33: goto tr5;
		case 102: goto st10;
		case 110: goto tr19;
	}
	if ( (*p) < 35 ) {
		if ( (*p) > 8 ) {
			if ( 14 <= (*p) && (*p) <= 31 )
				goto tr5;
		} else if ( (*p) >= 0 )
			goto tr5;
	} else if ( (*p) > 38 ) {
		if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else if ( (*p) >= 40 )
			goto tr5;
	} else
		goto tr5;
	goto tr15;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 33: goto tr5;
		case 102: goto tr20;
	}
	if ( (*p) < 35 ) {
		if ( (*p) > 8 ) {
			if ( 14 <= (*p) && (*p) <= 31 )
				goto tr5;
		} else if ( (*p) >= 0 )
			goto tr5;
	} else if ( (*p) > 38 ) {
		if ( (*p) > 91 ) {
			if ( 93 <= (*p) )
				goto tr5;
		} else if ( (*p) >= 40 )
			goto tr5;
	} else
		goto tr5;
	goto tr15;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3: goto tr0;
	case 4: goto tr13;
	case 0: goto tr0;
	case 5: goto tr14;
	case 1: goto tr0;
	case 6: goto tr0;
	case 7: goto tr15;
	case 8: goto tr15;
	case 9: goto tr15;
	case 10: goto tr15;
	}
	}

	}

/* #line 94 "./apps/cmd_line_parser.rl" */
    return p;
}

void cmd_line_parser::empty_callback(cmd_line_parser *p, event e) {

}

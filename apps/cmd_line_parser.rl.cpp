
/* #line 1 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
#include "cmd_line_parser.hpp"


/* #line 40 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */


namespace {

/* #line 12 "/media/amam/sources/zoal/apps/cmd_line_parser.rl.cpp" */
static const char _fsm_name_to_state_actions[] = {
	0, 0, 3, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const char _fsm_name_from_state_actions[] = {
	0, 0, 4, 0, 0, 0, 0, 0, 
	0, 0, 0
};

static const int fsm_name_start = 2;
static const int fsm_name_first_final = 2;
static const int fsm_name_error = -1;

static const int fsm_name_en_main = 2;


/* #line 44 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
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
    
/* #line 77 "/media/amam/sources/zoal/apps/cmd_line_parser.rl.cpp" */
	{
	cs = fsm_name_start;
	ts = 0;
	te = 0;
	act = 0;
	}

/* #line 90 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
}

const char* command_line_parse_machine::run_machine(const char *p, const char *pe) {
    
/* #line 90 "/media/amam/sources/zoal/apps/cmd_line_parser.rl.cpp" */
	{
	if ( p == pe )
		goto _test_eof;
_resume:
	switch ( _fsm_name_from_state_actions[cs] ) {
	case 4:
/* #line 1 "NONE" */
	{ts = p;}
	break;
/* #line 100 "/media/amam/sources/zoal/apps/cmd_line_parser.rl.cpp" */
	}

	switch ( cs ) {
case 2:
	switch( (*p) ) {
		case 10: goto tr6;
		case 32: goto tr7;
		case 34: goto tr8;
		case 43: goto tr9;
		case 45: goto tr9;
		case 109: goto tr11;
		case 111: goto tr12;
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
case 3:
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
case 4:
	switch( (*p) ) {
		case 9: goto tr1;
		case 92: goto tr3;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr1;
	goto tr13;
case 0:
	switch( (*p) ) {
		case 9: goto tr1;
		case 34: goto tr2;
		case 92: goto tr3;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr1;
	goto tr0;
case 5:
	switch( (*p) ) {
		case 9: goto tr1;
		case 34: goto tr2;
		case 92: goto tr3;
	}
	if ( 32 <= (*p) && (*p) <= 126 )
		goto tr1;
	goto tr14;
case 1:
	switch( (*p) ) {
		case 34: goto tr1;
		case 102: goto tr1;
		case 110: goto tr1;
		case 114: goto tr1;
		case 116: goto tr1;
		case 118: goto tr1;
	}
	goto tr0;
case 6:
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
case 7:
	switch( (*p) ) {
		case 33: goto tr5;
		case 115: goto tr16;
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
case 9:
	switch( (*p) ) {
		case 33: goto tr5;
		case 102: goto tr18;
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

	tr1: cs = 0; goto _again;
	tr3: cs = 1; goto _again;
	tr0: cs = 2; goto f0;
	tr4: cs = 2; goto f4;
	tr6: cs = 2; goto f6;
	tr7: cs = 2; goto f7;
	tr13: cs = 2; goto f10;
	tr14: cs = 2; goto f11;
	tr15: cs = 2; goto f12;
	tr5: cs = 3; goto f5;
	tr17: cs = 3; goto f13;
	tr19: cs = 3; goto f14;
	tr20: cs = 3; goto f15;
	tr8: cs = 4; goto f8;
	tr2: cs = 5; goto f1;
	tr9: cs = 6; goto f5;
	tr10: cs = 6; goto f9;
	tr11: cs = 7; goto _again;
	tr16: cs = 8; goto _again;
	tr12: cs = 9; goto _again;
	tr18: cs = 10; goto _again;

f7:
/* #line 33 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{te = p+1;}
	goto _again;
f6:
/* #line 34 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{te = p+1;{
            handler_(this, (int)command_line_event::line_end);
        }}
	goto _again;
f4:
/* #line 38 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{te = p+1;}
	goto _again;
f11:
/* #line 6 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{te = p;p--;{
		quoted_param_found_action();
	}}
	goto _again;
f12:
/* #line 30 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{te = p;p--;{
            handler_(this, (int)command_line_event::token);
        }}
	goto _again;
f10:
/* #line 38 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{te = p;p--;}
	goto _again;
f0:
/* #line 1 "NONE" */
	{	switch( act ) {
	case 1:
	{{p = ((te))-1;}
    	    handler_(this, (int)command_line_event::command_msg);
        }
	break;
	case 2:
	{{p = ((te))-1;}
            handler_(this, (int)command_line_event::command_on);
        }
	break;
	case 3:
	{{p = ((te))-1;}
            handler_(this, (int)command_line_event::command_off);
        }
	break;
	case 4:
	{{p = ((te))-1;}
            handler_(this, (int)command_line_event::token);;
        }
	break;
	case 5:
	{{p = ((te))-1;}
		quoted_param_found_action();
	}
	break;
	case 6:
	{{p = ((te))-1;}
            handler_(this, (int)command_line_event::token);
        }
	break;
	default:
	{{p = ((te))-1;}}
	break;
	}
	}
	goto _again;
f13:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 17 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 1;}
	goto _again;
f14:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 20 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 2;}
	goto _again;
f15:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 23 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 3;}
	goto _again;
f9:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 26 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 4;}
	goto _again;
f1:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 6 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 5;}
	goto _again;
f5:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 30 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 6;}
	goto _again;
f8:
/* #line 1 "NONE" */
	{te = p+1;}
/* #line 38 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
	{act = 9;}
	goto _again;

_again:
	switch ( _fsm_name_to_state_actions[cs] ) {
	case 3:
/* #line 1 "NONE" */
	{ts = 0;}
	break;
/* #line 433 "/media/amam/sources/zoal/apps/cmd_line_parser.rl.cpp" */
	}

	if ( ++p != pe )
		goto _resume;
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

/* #line 94 "/media/amam/sources/zoal/apps/cmd_line_parser.rl" */
    return p;
}

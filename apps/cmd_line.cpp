#include "cmd_line_parser.hpp"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <string>

int main( int argc, char **argv) {
    char msg[] = R"(a s sdfsdfd f sdfsdg h h j )";

    cmd_line_parser clp;

    char *star = msg;
    char *end = msg + strlen(msg) + 1;

    clp.do_parse(star, end);

    return 0;
}

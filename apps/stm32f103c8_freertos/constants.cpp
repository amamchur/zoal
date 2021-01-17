#include "constants.hpp"

const char *zoal_ascii_logo = "  __________          _\r\n"
                              " |___  / __ \\   /\\   | |\r\n"
                              "    / / |  | | /  \\  | |\r\n"
                              "   / /| |  | |/ /\\ \\ | |\r\n"
                              "  / /_| |__| / ____ \\| |____\r\n"
                              " /_____\\____/_/    \\_\\______|\r\n"
                              "\r\n";

const char *help_message = "ZOAL FreeRTOS Demo Application\r\n"
                           "Commands: \r\n"
                           "\t on\t\t\tturn led on\r\n"
                           "\t off\t\t\tturn led off\r\n"
                           "\t ticks\t\t\tprint current tick count\r\n"
                           "\t time\t\t\tprint RTC time\r\n"
                           "\t set-time [hh:mm::ss]\tset RTC time\r\n"
                           "\t heap\t\t\tFreeRTOS free heap size \r\n"
                           "\t task-info [name]\tprint task info (name: main; input, command)\r\n";

const char key_map[][3] = { //
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

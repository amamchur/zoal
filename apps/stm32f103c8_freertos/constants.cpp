#include "constants.hpp"

const char *zoal_ascii_logo = "  __________          _\r\n"
                              " |___  / __ \\   /\\   | |\r\n"
                              "    / / |  | | /  \\  | |\r\n"
                              "   / /| |  | |/ /\\ \\ | |\r\n"
                              "  / /_| |__| / ____ \\| |____\r\n"
                              " /_____\\____/_/    \\_\\______|\r\n"
                              "\r\n";

const char *help_message = "ZOAL STM32F103 FreeRTOS Demo Application\r\n"
                           "Commands: \r\n"
                           "\teeprom\t\t\t\tread eeprom\r\n"
                           "\theap\t\t\t\tFreeRTOS free heap size \r\n"
                           "\ti2c\t\t\t\tscan i2c devices\r\n"
                           "\tled 1\t\t\t\tturn led on/off\r\n"
                           "\ttask-info \"name\"\t\tprint task info (main; input, command)\r\n"
                           "\tticks\t\t\t\tprint current tick count\r\n"
                           "\ttime yyyy.MM.dd HH:mm::ss\tset RTC time\r\n"
                           "\ttime\t\t\t\tprint RTC time\r\n";

const char key_map[][3] = { //
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

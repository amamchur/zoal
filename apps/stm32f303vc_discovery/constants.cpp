#include "constants.hpp"

const char *zoal_ascii_logo = "  __________          _\r\n"
                              " |___  / __ \\   /\\   | |\r\n"
                              "    / / |  | | /  \\  | |\r\n"
                              "   / /| |  | |/ /\\ \\ | |\r\n"
                              "  / /_| |__| / ____ \\| |____\r\n"
                              " /_____\\____/_/    \\_\\______|\r\n"
                              "\r\n";

const char *help_message = "ZOAL FreeRTOS Demo Application\r\n"
                           "Commands:\r\n"
                           " axis\t\tdisplay axis\r\n"
                           " help\t\tdisplay help\r\n"
                           " i2c\t\tscan i2c devices\r\n"
                           " ledN 0|1\tturn led N off/on (N=3..10)\r\n"
                           " leds 0|1\tturn all leds off/on\r\n"
                           " ticks\t\tdisplay RTOS ticks\r\n";

const char key_map[][3] = { //
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

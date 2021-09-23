#include "./terminal.hpp"

#include "./command_machine.hpp"
#include "./hardware.hpp"

zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;

static const char *zoal_ascii_logo = "  __________          _\r\n"
                                     " |___  / __ \\   /\\   | |\r\n"
                                     "    / / |  | | /  \\  | |\r\n"
                                     "   / /| |  | |/ /\\ \\ | |\r\n"
                                     "  / /_| |__| / ____ \\| |____\r\n"
                                     " /_____\\____/_/    \\_\\______|\r\n"
                                     "\r\n";

static const char *help_message = "ZOAL STM32F030 FreeRTOS Demo Application\r\n"
                                  "Commands: \r\n"
                                  "\thelp\t\t\t\tthis message\r\n"
                                  "\tled 1\t\t\t\tturn led on/off\r\n";

static void handle_v100(const zoal::misc::terminal_input *, zoal::misc::terminal_machine_event e) {
    switch (e) {
    case zoal::misc::terminal_machine_event::up_key:
        break;
    case zoal::misc::terminal_machine_event::down_key:
        terminal.value("");
        break;
    default:
        break;
    }
}

static void command_callback(zoal::misc::command_machine *, zoal::misc::app_cmd cmd, int argc, zoal::misc::cmd_arg *argv) {
    switch (cmd) {
    case zoal::misc::app_cmd::help:
        stream << help_message;
        break;
    case zoal::misc::app_cmd::led:
        if ((int)*argv == 0) {
            ::led_pin::high();
        } else {
            ::led_pin::low();
        }
        break;
    default:
        break;
    }
}

static void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    stream << "\r\n";

    zoal::misc::command_machine cm;
    cm.callback(command_callback);
    cm.run_machine(s, e, e);

    terminal.sync();
}

static void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    usart_tx.send_data(s, e - s);
}

void init_terminal() {
    static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.handle_v100(&handle_v100);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    stream << zoal_ascii_logo << help_message;
    terminal.sync();
}

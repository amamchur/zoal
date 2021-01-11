#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include <zoal/freertos/event_group.hpp>
#include <zoal/freertos/queue.hpp>
#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/freertos/task.hpp>
#include <zoal/io/button.hpp>
#include <zoal/io/matrix_keypad.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/utils/cmsis_os2/tool_set.hpp>

using mcu = zoal::mcu::stm32f103c8tx<>;
using usart_01 = mcu::usart_01;

using api = zoal::gpio::api;
using tools = zoal::utils::cmsis_os2::tool_set<mcu, void>;
using user_led = mcu::pc_13;

enum app_cmd : uint32_t {
    app_cmd_none,
    app_cmd_led_on,
    app_cmd_led_off,
    app_cmd_ticks,
    app_cmd_button1_pressed,
    app_cmd_button2_pressed,
    app_cmd_print_heap_size,
    app_cmd_task_info,
    app_cmd_help,
    app_cmd_not_found
};

[[noreturn]] void zoal_input_processor(void *);
[[noreturn]] void zoal_cmd_processor(void *);

using row_selector = zoal::io::keypad_row_selector<mcu::pa_00, mcu::pa_01, mcu::pa_02, mcu::pa_03>;
using column_reader = zoal::io::keypad_column_reader<mcu::pa_04, mcu::pa_05, mcu::pa_06>;
using keypad_type = zoal::io::matrix_keypad<TickType_t, row_selector, column_reader>;

auto zoal_ascii_logo = "  __________          _\r\n"
                       " |___  / __ \\   /\\   | |\r\n"
                       "    / / |  | | /  \\  | |\r\n"
                       "   / /| |  | |/ /\\ \\ | |\r\n"
                       "  / /_| |__| / ____ \\| |____\r\n"
                       " /_____\\____/_/    \\_\\______|\r\n"
                       "\r\n";
auto help_message = "ZOAL FreeRTOS Demo Application\r\n"
                    "Commands: \r\n"
                    "\t on\t\t\tturn led on\r\n"
                    "\t off\t\t\tturn led off\r\n"
                    "\t ticks\t\t\tprint current tick count\r\n"
                    "\t heap\t\t\tFreeRTOS free heap size \r\n"
                    "\t task-info [name]\tprint task info (name: main; input, command)\r\n"
                    "\r\n";
const char key_map[][3] = { //
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
keypad_type keypad;

zoal::io::button<TickType_t, mcu::pb_12> user_button_1;
zoal::io::button<TickType_t, mcu::pb_13> user_button_2;

using command_line_parser = zoal::misc::command_line_parser;
constexpr size_t input_string_length = 256;
zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
char command_history[input_string_length] = {0};

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
using queue_type = zoal::freertos::queue<app_cmd, zoal::freertos::freertos_allocation_type::static_mem>;
using event_group_type = zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem>;
using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;

zoal::mem::reserve_mem<stream_buffer_type, 32> rx_stream_buffer(1);
zoal::mem::reserve_mem<stream_buffer_type, 32> tx_stream_buffer(1);
zoal::mem::reserve_mem<queue_type, 5, queue_type::item_type> command_queue;

__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> input_task(zoal_input_processor, "input");

class usart_01_tx_transport {
public:
    static void push_back_blocking(uint8_t value) {
        tx_stream_buffer.send(value, portMAX_DELAY);
        usart_01 ::enable_tx();
    }
};

using stream_type = zoal::io::output_stream<usart_01_tx_transport>;
stream_type tx_stream;

TaskStatus_t taskStatus;

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        app_cmd command = app_cmd_none;
        command_queue.pop(command);
        if (command == app_cmd_none) {
            continue;
        }

        tx_stream << "\r\n";
        switch (command) {
        case app_cmd_led_on:
            user_led::low();
            break;
        case app_cmd_led_off:
            user_led::high();
            break;
        case app_cmd_ticks:
            tx_stream << xTaskGetTickCount() << "\r\n";
            break;
        case app_cmd_button1_pressed:
            tx_stream << "button1_pressed\r\n";
            break;
        case app_cmd_button2_pressed:
            tx_stream << "button2_pressed\r\n";
            break;
        case app_cmd_not_found:
            tx_stream << "Not found\r\n";
            break;
        case app_cmd_print_heap_size:
            tx_stream << "free heap size: " << xPortGetFreeHeapSize() << "\r\n";
            break;
        case app_cmd_help:
            tx_stream << help_message;
            break;
        case app_cmd_task_info:
            tx_stream << "pcTaskName:\t\t" << taskStatus.pcTaskName << "\r\n";
            tx_stream << "eCurrentState:\t\t" << taskStatus.eCurrentState << "\r\n";
            tx_stream << "uxCurrentPriority:\t" << taskStatus.uxCurrentPriority << "\r\n";
            tx_stream << "uxBasePriority:\t\t" << taskStatus.uxBasePriority << "\r\n";
            tx_stream << "ulRunTimeCounter:\t" << taskStatus.ulRunTimeCounter << "\r\n";
            tx_stream << "pxStackBase:\t\t" << (void *)taskStatus.pxStackBase << "\r\n";
            tx_stream << "usStackHighWaterMark\t" << taskStatus.usStackHighWaterMark << "\r\n";
            break;
        default:
            break;
        }
        terminal.sync();
    }
}

[[noreturn]] void zoal_input_processor(void *) {
    auto button_callback = [](zoal::io::button_event e, app_cmd cmd) {
        if (e == zoal::io::button_event::press) {
            command_queue.push(cmd);
        }
    };

    auto keypad_callback = [](zoal::io::button_event e, size_t r, size_t c) {
        if (e == zoal::io::button_event::press) {
            rx_stream_buffer.send(key_map[r][c]);
        }
    };

    while (true) {
        auto time = xTaskGetTickCount();
        user_button_1.handle(time, button_callback, app_cmd_button1_pressed);
        user_button_2.handle(time, button_callback, app_cmd_button2_pressed);
        keypad.handle(keypad_callback, time);
        vTaskDelay(10);
    }
}

extern "C" __unused void zoal_init_hardware() {
    api::optimize<api::power_on<usart_01, mcu::port_a, mcu::port_c, mcu::port_b>>();
    api::optimize<api::disable<usart_01>>();

    api::optimize<
        //
        mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::connect,
        mcu::cfg::usart<usart_01, 115200>::cfg,
        //
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12, mcu::pb_13>,
        api::mode<zoal::gpio::pin_mode::output, user_led>
        //
        >();

    api::optimize<api::enable<usart_01>>();
    usart_01::enable_rx();

    HAL_NVIC_SetPriority(USART1_IRQn, 13, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void vt100_callback(const zoal::misc::terminal_input *, const char *s, const char *) {
    for (auto ch = s; *ch; ch++) {
        usart_01_tx_transport::push_back_blocking(*ch);
    }
}

template<class T>
bool cmp_str_token(T s1, const char *ss, const char *se) {
    while (*s1 && ss < se) {
        if (*s1 != *ss) {
            return false;
        }
        ++s1;
        ++ss;
    }
    return !*s1 && ss == se;
}

void take_task_name(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    char name[16] = {0};
    auto end = name + sizeof(name) - 1;
    auto src = ts;
    auto dst = name;
    while (src != te && dst != end) *dst++ = *src++;
    *dst = 0;

    TaskHandle_t xTask = xTaskGetHandle(name);
    if (xTask) {
        vTaskGetInfo(xTask, &taskStatus, pdTRUE, eInvalid);
        command_queue.push(app_cmd_task_info);
    } else {
        command_queue.push(app_cmd_not_found);
    }
}

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    p->callback(&command_line_parser::empty_callback);
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    app_cmd command;

    if (cmp_str_token("on", ts, te)) {
        command = app_cmd_led_on;
    } else if (cmp_str_token("off", ts, te)) {
        command = app_cmd_led_off;
    } else if (cmp_str_token("ticks", ts, te)) {
        command = app_cmd_ticks;
    } else if (cmp_str_token("heap", ts, te)) {
        command = app_cmd_print_heap_size;
    } else if (cmp_str_token("help", ts, te)) {
        command = app_cmd_help;
    } else if (cmp_str_token("task-info", ts, te)) {
        p->callback(&take_task_name);
        return;
    } else {
        command = app_cmd_not_found;
    }
    command_queue.push(command);
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    auto src = s;
    auto dst = command_history;
    while (src != e) *dst++ = *src++;
    *dst = 0;

    command_line_parser parser(nullptr, 0);
    parser.callback(cmd_select_callback);
    parser.scan(s, e, e);
}

void handle_v100(const zoal::misc::terminal_input *, zoal::misc::terminal_machine_event e) {
    switch (e) {
    case zoal::misc::terminal_machine_event::ctrl_up_key:
    case zoal::misc::terminal_machine_event::up_key:
        terminal.value(command_history);
        break;
    case zoal::misc::terminal_machine_event::ctrl_down_key:
    case zoal::misc::terminal_machine_event::down_key:
        terminal.value("");
        break;
    default:
        break;
    }
}

extern "C" [[noreturn]] __unused void zoal_default_thread(void *) {
    static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.handle_v100(&handle_v100);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    tx_stream << zoal_ascii_logo << help_message;

    terminal.sync();

    for (;;) {
        uint8_t rx_buffer[8];
        auto size = rx_stream_buffer.receive(rx_buffer, sizeof(rx_buffer));
        if (size > 0) {
            terminal.push(rx_buffer, size);
        }
    }
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler([](uint8_t &value) { return tx_stream_buffer.receive_isr(&value, 1) > 0; });
    usart_01::rx_handler([](uint8_t byte) { rx_stream_buffer.send_isr(byte); });
}

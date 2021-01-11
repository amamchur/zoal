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
    app_cmd_ping,
    app_cmd_led_on,
    app_cmd_led_off,
    app_cmd_ticks,
    app_cmd_button1_pressed,
    app_cmd_button2_pressed,
    app_cmd_print_watermark,
    app_cmd_print_heap_size,
    app_cmd_not_found
};

[[noreturn]] void zoal_input_processor(void *);
[[noreturn]] void zoal_cmd_processor(void *);

using row_selector = zoal::io::keypad_row_selector<mcu::pa_00, mcu::pa_01, mcu::pa_02, mcu::pa_03>;
using column_reader = zoal::io::keypad_column_reader<mcu::pa_04, mcu::pa_05, mcu::pa_06>;
using keypad_type = zoal::io::matrix_keypad<TickType_t, row_selector, column_reader>;

const char key_map[][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
keypad_type keypad;

zoal::io::button<TickType_t, mcu::pb_12> user_button_1;
zoal::io::button<TickType_t, mcu::pb_13> user_button_2;

using command_line_parser = zoal::misc::command_line_parser;
constexpr size_t input_string_length = 128;
zoal::mem::reserve_mem<zoal::misc::terminal_input, input_string_length> terminal;
char cmd_history[input_string_length] = {0};

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
using queue_type = zoal::freertos::queue<app_cmd, zoal::freertos::freertos_allocation_type::static_mem>;
using event_group_type = zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem>;
using stream_buffer_type = zoal::freertos::stream_buffer<zoal::freertos::freertos_allocation_type::static_mem>;

zoal::mem::reserve_mem<stream_buffer_type, 16> rx_stream_buffer(1);
zoal::mem::reserve_mem<stream_buffer_type, 16> tx_stream_buffer(1);
zoal::mem::reserve_mem<queue_type, 3, queue_type::item_type> cmd_queue;

__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> cmd_processor_task(zoal_cmd_processor);
__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> input_processor_task(zoal_input_processor);

class usart_01_tx_transport {
public:
    static void push_back_blocking(uint8_t value) {
        tx_stream_buffer.send(value, portMAX_DELAY);
        usart_01 ::enable_tx();
    }
};

using stream_type = zoal::io::output_stream<usart_01_tx_transport>;
stream_type tx_stream;

volatile UBaseType_t uxHighWaterMark = 0;
//uxHighWaterMark = uxTaskGetStackHighWaterMark(nullptr);

[[noreturn]] void zoal_cmd_processor(void *) {
    while (true) {
        app_cmd command = app_cmd_none;
        cmd_queue.pop(command);
        if (command == app_cmd_none) {
            continue;
        }

        tx_stream << "\r\n";
        switch (command) {
        case app_cmd_ping:
            tx_stream << "pong!\r\n";
            break;
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
            tx_stream << "app_cmd_not_found\r\n";
            break;
        case app_cmd_print_watermark:
            tx_stream << "wm: " << uxHighWaterMark << "\r\n";
            break;
        case app_cmd_print_heap_size:
            tx_stream << "free heap size: " << xPortGetFreeHeapSize() << "\r\n";
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
            cmd_queue.push(cmd);
        }
    };

    auto keypad_callback = [](zoal::io::button_event e, size_t r, size_t c) {
        if (e == zoal::io::button_event::press) {
            tx_stream << "Pressed: " << r << ", " << c << ", \"" << key_map[r][c] << "\"\r\n" ;
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

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto ts = p->token_start();
    auto te = p->token_end();
    auto task = app_cmd_none;
    auto src = ts;
    auto dst = cmd_history;
    while (src != te) *dst++ = *src++;
    *dst = 0;

    p->callback(&command_line_parser::empty_callback);
    if (cmp_str_token("ping", ts, te)) {
        task = app_cmd_ping;
    } else if (cmp_str_token("on", ts, te)) {
        task = app_cmd_led_on;
    } else if (cmp_str_token("off", ts, te)) {
        task = app_cmd_led_off;
    } else if (cmp_str_token("ticks", ts, te)) {
        task = app_cmd_ticks;
    } else if (cmp_str_token("wm", ts, te)) {
        task = app_cmd_print_watermark;
    } else if (cmp_str_token("heap", ts, te)) {
        task = app_cmd_print_heap_size;
    } else {
        task = app_cmd_not_found;
    }
    cmd_queue.push(task);
}

void input_callback(const zoal::misc::terminal_input *, const char *s, const char *e) {
    command_line_parser cmd_parser(nullptr, 0);
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.scan(s, e, e);
}

void handle_v100(const zoal::misc::terminal_input *,  zoal::misc::terminal_machine_event e) {
    switch (e) {
    case zoal::misc::terminal_machine_event::ctrl_up_key:
    case zoal::misc::terminal_machine_event::up_key:
        terminal.value(cmd_history);
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

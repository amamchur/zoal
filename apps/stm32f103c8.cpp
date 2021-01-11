#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "task.h"

#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/io/button.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/utils/cmsis_os2/delay.hpp>
#include <zoal/utils/cmsis_os2/tool_set.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/vt100.hpp>

#include "../misc/cmd_line_parser.hpp"
#include "../misc/terminal_input.hpp"

using mcu = zoal::mcu::stm32f103c8tx<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = usart_01::default_tx_buffer<128>;
using usart_01_rx_buffer = usart_01::default_rx_buffer<128>;
using logger = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::cmsis_os2::tool_set<mcu, logger>;
using delay = zoal::utils::cmsis_os2::delay<mcu>;
using api = zoal::gpio::api;
using user_led = mcu::pc_13;

constexpr size_t terminal_str_size = 128;
using command_line_parser = zoal::misc::command_line_parser<terminal_str_size>;
char terminal_buffer[terminal_str_size];
auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
zoal::misc::terminal_input terminal(terminal_buffer, sizeof(terminal_buffer));
command_line_parser cmd_parser;

zoal::io::button<tools, mcu::pb_13> user_button_1;
zoal::io::button<tools, mcu::pb_12> user_button_2;

[[noreturn]] void zoal_input_processor(void *) {
    while (true) {
//        auto events = user_button_1.handle();
//        if ((events & zoal::io::button_state_trigger_press) != 0) {
//            logger::stream() << zoal::utils::vt100::ris();
//        }
//
//        events = user_button_2.handle();
//        if ((events & zoal::io::button_state_trigger_press) != 0) {
//            mcu::pc_13::toggle();
//        }

        osDelay(1);
    }
}

extern "C" void zoal_init() {
    api::optimize<api::power_on<usart_01, mcu::port_a, mcu::port_c, mcu::port_b>>();
    api::optimize<api::disable<usart_01>>();

    api::optimize<
        //
        mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::connect,
        mcu::cfg::usart<usart_01, 115200>::cfg,
        //
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12, mcu::pb_13>,
        api::mode<zoal::gpio::pin_mode::output, mcu::pc_13>
        //
        >();

    api::optimize<api::enable<usart_01>>();
    usart_01::enable_rx();

    NVIC_EnableIRQ(USART1_IRQn);

    xTaskCreate(zoal_input_processor, nullptr, 128, nullptr, osPriorityNormal, nullptr);
}

void vt100_callback(const zoal::misc::terminal_input *t, const char *s, const char *) {
    for (auto ch = s; *ch; ch++) {
        usart_01_tx_buffer::push_back_blocking(*ch);
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

void cmd_select_callback(void *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        return;
    }

    auto parser = reinterpret_cast<command_line_parser *>(p);
    auto ts = parser->token_start();
    auto te = parser->token_end();
    if (cmp_str_token("help", ts, te)) {
        auto stream = logger::stream();
        stream << "\r\nHello!";
        parser->callback(&command_line_parser::empty_callback);
        return;
    }

    if (cmp_str_token("on", ts, te)) {
        user_led::low();
        return;
    }

    if (cmp_str_token("off", ts, te)) {
        user_led::high();
        return;
    }

    parser->callback(&command_line_parser::empty_callback);
    logger::warn() << "Not found";
}

void input_callback(const zoal::misc::terminal_input *t, const char *s, const char *e) {
    cmd_parser.reset();
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.push(s, e - s, e);
}

extern "C" [[noreturn]] void zoal_default_thread(void *argument) {
    zoal_init();
    logger::info() << "Started!";

    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.greeting(terminal_greeting);
    terminal.clear();
    terminal.sync();

    for (;;) {
        uint8_t rx_byte = 0;
        auto result = usart_01_rx_buffer::pop_front(rx_byte);
        if (result) {
            terminal.push(&rx_byte, 1);
        }
        osThreadYield();
    }
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler<usart_01_tx_buffer>();
    usart_01::rx_handler<usart_01_rx_buffer>();
}

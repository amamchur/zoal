#include "../../misc/type_detector.hpp"
#include "../misc/terminal_input.hpp"
#include "command_processor.hpp"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "stm32f1xx_hal.h"
#include "terminal.hpp"
#include "rtc.h"
#include "gpio.h"

#include <zoal/freertos/task.hpp>

[[noreturn]] void zoal_default_thread(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_default_thread, "main");

struct cas_print_functor {
    template<class A>
    void operator()() {
        tx_stream << "CAS: *" << (void *)A::address << " & ~" << (void *)A::clear << " | " << (void *)A::set << "\r\n";
    }
};

[[noreturn]] void zoal_default_thread(void *) {
    static auto terminal_greeting = "\033[0;32mmcu\033[m$ ";
    terminal.vt100_feedback(&vt100_callback);
    terminal.input_callback(&input_callback);
    terminal.handle_v100(&handle_v100);
    terminal.greeting(terminal_greeting);
    terminal.clear();

    tx_stream << zoal_ascii_logo;
    for (int i = 0; i < 60; ++i) {
        tx_stream << '.';
        vTaskDelay(20);
    }

    tx_stream << "\r\033[2K" << help_message << "Current time: ";
    print_time();

    //    using cas = mcu::cfg::usart<usart_01, zoal::periph::usart_115200<36000000>>::apply;
    //    cas_print_functor fn;
    //    zoal::ct::type_list_iterator<cas>::for_each(fn);

    terminal.sync();

    for (;;) {
        uint8_t rx_buffer[8];
        auto size = rx_stream_buffer.receive(rx_buffer, sizeof(rx_buffer));
        if (size > 0) {
            terminal.push(rx_buffer, size);
        }
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_RTC_Init();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

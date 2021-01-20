#include "command_processor.hpp"
#include "command_queue.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "rtc.h"
#include "stm32f1xx_hal.h"
#include "terminal.hpp"

#include <cstring>
#include <zoal/freertos/task.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/ms_counter.hpp>

using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<72000000, ms_counter>;

[[noreturn]] void zoal_main_task(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> terminal_task(zoal_terminal_rx_task, "terminal");
__unused zoal::mem::reserve_mem<task_type, 64, StackType_t> main_task(zoal_main_task, "main");

struct cas_print_functor {
    template<class A>
    void operator()() {
        tx_stream << "CAS: *" << (void *)A::address << " & ~" << (void *)A::clear << " | " << (void *)A::set << "\r\n";
    }
};

static void send_fast_commands() {
    static const char fast_cmd[][32] = {
        //
        "time\r",
        "heap\r",
        "on\r",
        "off\r",
        "task-info main\r",
        "task-info command\r",
        "task-info input\r"
        //
    };

    vTaskDelay(3);

    for (auto str : fast_cmd) {
        size_t length = std::strlen(str);
        rx_stream_buffer.send_all(str, length);
        vTaskDelay(50);
    }
}

[[noreturn]] void zoal_main_task(void *) {
    //    send_fast_commands();

    for (;;) {
        user_led::on();
        vTaskDelay(500);
        user_led::off();
        vTaskDelay(500);
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

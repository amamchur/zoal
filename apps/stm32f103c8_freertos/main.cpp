#include "command_processor.hpp"
#include "command_queue.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "stm32f1xx_hal.h"
#include "terminal.hpp"

#include <zoal/freertos/task.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/ms_counter.hpp>

using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<72000000, ms_counter>;

[[noreturn]] void zoal_main_task(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;

__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

[[noreturn]] void zoal_main_task(void *) {
    init_terminal();

    for (;;) {
        auto bits = hardware_events.wait(all_hardware_events);
        if ((bits & i2c_event) == i2c_event) {
            i2c_req_dispatcher.handle();
        }

        if ((bits & usart_event) == usart_event) {
            uint8_t rx_buffer[8];
            size_t size;
            do {
                size = rx_stream_buffer.receive(rx_buffer, sizeof(rx_buffer), 0);
                if (size != 0) {
                    terminal.push(rx_buffer, size);
                }
            } while (size == sizeof(rx_buffer));
        }
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

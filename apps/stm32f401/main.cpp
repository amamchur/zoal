#include "gpio.h"
#include "stm32f4xx_hal.h"

#include <zoal/freertos/task.hpp>
#include <zoal/mcu/stm32f401ccux.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/ms_counter.hpp>

using mcu = zoal::mcu::stm32f401ccux;

[[noreturn]] void zoal_main_task(void *);

using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<84000000, ms_counter>;
using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;

__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

[[noreturn]] void zoal_main_task(void *) {
    while (true) {
        mcu::pc_13::_0();
        vTaskDelay(100);
        mcu::pc_13::_1();
        vTaskDelay(100);
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    vTaskStartScheduler();
    return 0;
}

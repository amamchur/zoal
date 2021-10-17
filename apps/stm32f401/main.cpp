//
// Created by andrii on 17.10.21.
//

#include "stm32f4xx_hal.h"

#include <zoal/freertos/task.hpp>
#include <zoal/mcu/stm32f401ccux.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/mem/reserve_mem.hpp>

using mcu = zoal::mcu::stm32f401ccux;

[[noreturn]] void zoal_main_task(void *);

using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<72000000, ms_counter>;
using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;

zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

[[noreturn]] void zoal_main_task(void *) {
    while (true) {
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    vTaskStartScheduler();
    return 0;
}

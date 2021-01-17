#include "../../misc/type_detector.hpp"
#include "../misc/terminal_input.hpp"
#include "command_processor.hpp"
#include "command_queue.hpp"
#include "constants.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "rtc.h"
#include "stm32f1xx_hal.h"
#include "terminal.hpp"
#include "tim.h"

#include <zoal/arch/cortex/stm32f1/timer.hpp>
#include <zoal/freertos/task.hpp>

using timer_04 = mcu::timer_04;

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

TIM_TypeDef buff1;
TIM_TypeDef buff2;
TIM_TypeDef buff3;

void print_data_diff(const void *d1, const void *d2, size_t size) {
    auto ptr1 = reinterpret_cast<const uint32_t *>(d1);
    auto ptr2 = reinterpret_cast<const uint32_t *>(d2);
    auto end = reinterpret_cast<const uint32_t *>(reinterpret_cast<const uint8_t *>(d1) + size);
    while (ptr1 < end) {
        auto offset = reinterpret_cast<const uint8_t *>(ptr1) - reinterpret_cast<const uint8_t *>(d1);
        bool equal = *ptr1 == *ptr2;
        if (!equal) {
            tx_stream << (void *)offset << " " << *ptr1 << "\t" << *ptr2 << "\r\n";
        }
        ptr1++;
        ptr2++;
    }
}

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

#if 0
    MX_TIM4_Init();
    HAL_TIM_Base_Start_IT(&htim4);
#else
    __HAL_RCC_TIM4_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);

//    MX_TIM4_Init();

    using timer_cfg = zoal::periph::timer_config<0, 7199, 99, zoal::periph::timer_mode::up>;
    using cas = mcu::cfg::timer<timer_04, timer_cfg>::apply;
    zoal::mem::apply_cas_list<cas>::apply();

    timer_04::enable_update_irq();
    timer_04::enable();
#endif

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

extern "C" void TIM4_IRQHandler() {
    timer_04::update_handler([](){
       mcu::pa_08::toggle();
    });
}

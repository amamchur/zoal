#include "../../misc/type_detector.hpp"
#include "../misc/terminal_input.hpp"
#include "command_processor.hpp"
#include "command_queue.hpp"
#include "constants.hpp"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "rtc.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_adc.h"
#include "terminal.hpp"

#include <algorithm>
#include <zoal/arch/cortex/stm32f1/adc.hpp>
#include <zoal/freertos/task.hpp>

using adc = zoal::arch::stm32f1::adc<0x40012400>;

volatile uint32_t adc_value;

[[noreturn]] static void zoal_adc_read(void *) {
    adc::enable();

    int wait_loop_index = ((LL_ADC_DELAY_ENABLE_CALIB_ADC_CYCLES * 32) >> 1);
    while (wait_loop_index != 0) {
        wait_loop_index--;
    }
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0) {
    }

    while (true) {
        adc::start();
        adc::wait();
        adc_value = adc::read();
        tx_stream << adc_value << "\r\n";
        vTaskDelay(1000);
    }
}

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

    tx_stream << zoal_ascii_logo << help_message;

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

    MX_RTC_Init();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}


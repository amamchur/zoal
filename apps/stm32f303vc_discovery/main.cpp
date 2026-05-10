#include "./command_processor.hpp"
#include "./constants.hpp"
#include "./hardware.hpp"
#include "./input_processor.hpp"
#include "./terminal.hpp"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "stm32f3xx_hal.h"
#include "usart.h"

#include <zoal/freertos/event_group.hpp>
#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/freertos/task.hpp>
#include <zoal/ic/lsm303dlhc.hpp>
#include <zoal/mcu/stm32f303vctx.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/new.hpp>

using mcu = zoal::mcu::stm32f303vctx;
using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using i2c_01 = mcu::i2c_01;

[[noreturn]] void zoal_main_task(void *);
[[noreturn]] void zoal_leds(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
__unused zoal::mem::reserve_mem<task_type, 512, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> terminal_task(zoal_terminal_rx_task, "terminal");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
//__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> leds_task(zoal_leds, "leds");

[[noreturn]] void zoal_leds(void *) {
    constexpr TickType_t pause_ms = 100;
    while (true) {
        pcb::led_04::off();
        pcb::led_03::on();
        vTaskDelay(pause_ms);

        pcb::led_03::off();
        pcb::led_05::on();
        vTaskDelay(pause_ms);

        pcb::led_05::off();
        pcb::led_07::on();
        vTaskDelay(pause_ms);

        pcb::led_07::off();
        pcb::led_09::on();
        vTaskDelay(pause_ms);

        pcb::led_09::off();
        pcb::led_10::on();
        vTaskDelay(pause_ms);

        pcb::led_10::off();
        pcb::led_08::on();
        vTaskDelay(pause_ms);

        pcb::led_08::off();
        pcb::led_06::on();
        vTaskDelay(pause_ms);

        pcb::led_06::off();
        pcb::led_04::on();
        vTaskDelay(pause_ms);
    }
}

[[noreturn]] void zoal_main_task(void *) {
    for (;;) {
        auto bits = io_events.wait(1);
        if (bits) {
            i2c_dispatcher.handle();
        }
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
//    MX_USART3_UART_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

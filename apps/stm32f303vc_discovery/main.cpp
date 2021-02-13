#include "./constants.hpp"
#include "./hardware.hpp"
#include "./input_processor.hpp"
#include "./terminal.hpp"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "stm32f3xx_hal.h"
#include "usart.h"
#include "usb.h"

#include <zoal/freertos/stream_buffer.hpp>
#include <zoal/freertos/task.hpp>
#include <zoal/mcu/stm32f303vctx.hpp>
#include <zoal/mem/reserve_mem.hpp>
#include <zoal/utils/ms_counter.hpp>

using mcu = zoal::mcu::stm32f303vctx;
using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;

[[noreturn]] void zoal_main_task(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> terminal_task(zoal_terminal_rx_task, "terminal");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

using led_gpio_cfg = zoal::gpio::api::optimize<
    //
    led_03::gpio_cfg,
    led_04::gpio_cfg,
    led_05::gpio_cfg,
    led_06::gpio_cfg,
    led_07::gpio_cfg,
    led_08::gpio_cfg,
    led_09::gpio_cfg,
    led_10::gpio_cfg>;

[[noreturn]] void zoal_main_task(void *) {
    led_gpio_cfg();
    vTaskDelay(1000);

    while (true) {
        led_03::pin::toggle();
        vTaskDelay(100);

        led_04::pin::toggle();
        vTaskDelay(100);

        led_05::pin::toggle();
        vTaskDelay(100);

        led_06::pin::toggle();
        vTaskDelay(100);

        led_07::pin::toggle();
        vTaskDelay(100);

        led_08::pin::toggle();
        vTaskDelay(100);

        led_09::pin::toggle();
        vTaskDelay(100);

        led_10::pin::toggle();
        vTaskDelay(100);
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART3_UART_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_USB_PCD_Init();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include "task.h"

#include <zoal/io/button.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/utils/logger.hpp>

using mcu = zoal::mcu::stm32f103c8tx;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using api = zoal::gpio::api;
using user_led = mcu::pc_13;

[[noreturn]] void zoal_input_processor(void *) {
    while (true) {
        vTaskDelay(1);
    }
}

extern "C" void zoal_init() {
    using usart_01_cfg = zoal::periph::usart_115200<72000000>;

    api::optimize<api::clock_on<usart_01, mcu::port_a, mcu::port_c, mcu::port_b>>();
    api::optimize<api::disable<usart_01>>();

    api::optimize<
        //
        mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::connect,
        mcu::cfg::usart<usart_01, usart_01_cfg>::apply,
        //
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12, mcu::pb_13>,
        api::mode<zoal::gpio::pin_mode::output, mcu::pc_13>
        //
        >();

    api::optimize<api::enable<usart_01>>();
    //    usart_01::enable_rx();

    NVIC_EnableIRQ(USART1_IRQn);

    xTaskCreate(zoal_input_processor, nullptr, 128, nullptr, osPriorityNormal, nullptr);
}

extern "C" [[noreturn]] __unused void zoal_default_thread(void *) {
    zoal_init();

    for (;;) {
        vTaskDelay(1000);
    }
}

extern "C" void USART1_IRQHandler() {}

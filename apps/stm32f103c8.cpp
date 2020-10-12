#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/io/button.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/periph/rx_ring_buffer.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/utils/cmsis_os2/delay.hpp>
#include <zoal/utils/cmsis_os2/tool_set.hpp>
#include <zoal/utils/logger.hpp>

using mcu = zoal::mcu::stm32f103c8tx<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = usart_01::default_tx_buffer<128>;
using logger = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::cmsis_os2::tool_set<mcu, logger>;
using delay = zoal::utils::cmsis_os2::delay<mcu>;
using api = zoal::gpio::api;

osThreadId_t inputTaskHandle;
osThreadAttr_t inputTask_attributes;

zoal::io::button<tools, mcu::pb_12> user_button;

#define DEMCR (*((volatile uint32_t *)0xE000EDFC))
#define DWT_CTRL (*(volatile uint32_t *)0xe0001000)
#define CYCCNTENA (1 << 0)
#define DWT_CYCCNT ((volatile uint32_t *)0xE0001004)
#define CPU_CYCLES *DWT_CYCCNT

template<int32_t Value>
void do_test() {
    uint32_t diff = 0;
    uint32_t overhead = 0;
    {
        zoal::utils::interrupts ni(false);
        CPU_CYCLES = 0;
        DWT_CTRL |= CYCCNTENA;
        DWT_CTRL &= ~CYCCNTENA;
        overhead = CPU_CYCLES;
    }
    /*
     * 0 - 7
     * 1 - 7
     * 2 - 9
     * 3 - 15
     */
    {
        zoal::utils::interrupts ni(false);
        CPU_CYCLES = 0;
        DWT_CTRL |= CYCCNTENA;
        while (CPU_CYCLES < Value)
            ;
        DWT_CTRL &= ~CYCCNTENA;
        diff = CPU_CYCLES - overhead;
    }

    logger::info() << "v: " << Value << "; ops: " << diff;
}

[[noreturn]] void zoal_input_handler(void *) {
    while (1) {
        auto events = user_button.handle();
        if ((events & zoal::io::button_state_trigger_press) != 0) {
            mcu::pc_13::toggle();
        }

        osThreadYield();
    }
}

extern "C" void zoal_init() {
    inputTask_attributes.name = "inputHandler";
    inputTask_attributes.priority = osPriorityLow;
    inputTask_attributes.stack_size = 128 * 4;

    inputTaskHandle = osThreadNew(zoal_input_handler, NULL, &inputTask_attributes);

    api::optimize<api::power_on<usart_01, mcu::port_a, mcu::port_c>>();
    api::optimize<api::disable<usart_01>>();

    api::optimize<
        //
        mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::on_cas,
        //
        mcu::cfg::usart<usart_01, 115200>::cfg,
        //
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12>,
        api::mode<zoal::gpio::pin_mode::output, mcu::pc_13>
        //
        >();

    api::optimize<api::enable<usart_01, mcu::port_a, mcu::port_c>>();

    NVIC_EnableIRQ(USART1_IRQn);
}

extern "C" [[noreturn]] void zoal_default_thread(void *argument) {
    logger::info() << "----- Started CubeMX-----";

    constexpr int delay = 3000;
    for (;;) {
        logger::info() << "Heartbeat: " << osKernelGetTickCount();
        ::delay::ms(delay);
    }
}

extern "C" [[noreturn]] void zoal_main() {
    while (1) {
    }
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler<usart_01_tx_buffer>();
}

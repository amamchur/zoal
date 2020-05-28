#include "cmsis_os.h"
#include "stm32f1xx_hal.h"

#include <zoal/io/button.hpp>

#include <zoal/data/rx_tx_buffer.hpp>
#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/periph/rx_ring_buffer.hpp>
#include <zoal/periph/tx_ring_buffer.hpp>
#include <zoal/utils/cmsis_os2/delay.hpp>
#include <zoal/utils/logger.hpp>
#include <zoal/utils/cmsis_os2/tool_set.hpp>

using mcu = zoal::mcu::stm32f103c8tx<>;
using usart_01 = mcu::usart_01;
using usart_02 = mcu::usart_02;
using usart_03 = mcu::usart_03;

using usart_01_tx_buffer = usart_01::default_tx_buffer<128>;
using logger_01 = zoal::utils::terminal_logger<usart_01_tx_buffer, zoal::utils::log_level::trace>;
using tools = zoal::utils::cmsis_os2::tool_set<mcu, logger_01>;
using delay = zoal::utils::cmsis_os2::delay<mcu>;

osThreadId_t inputTaskHandle;
osThreadAttr_t inputTask_attributes;

zoal::io::button<tools, mcu::pb_12> user_button;

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

    mcu::power<usart_01, mcu::port_a, mcu::port_c>::on();
    mcu::pb_12::mode<zoal::gpio::pin_mode::input_pull_up>();
    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();

    mcu::cfg::usart<usart_01, 115200>::apply();
    mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::on();

    usart_01::enable();
    NVIC_EnableIRQ(USART1_IRQn);
}

extern "C" [[noreturn]] void zoal_default_thread(void *argument) {
    logger_01::info() << "----- Started CubeMX-----";

    constexpr int delay = 3000;
    for (;;) {
        logger_01::info() << "Heartbeat: " << osKernelGetTickCount();
        ::delay::ms(delay);
    }
}

extern "C" [[noreturn]] void zoal_main() {
    //    using namespace zoal::gpio;
    //
    //    mcu::power<mcu::port_a, mcu::port_b, mcu::port_c>::on();
    //    mcu::pc_13::mode<zoal::gpio::pin_mode::output>();
    //
    //    constexpr int delay = 100;
    //
    //    while (true) {
    //        mcu::pc_13::high();
    //        ::delay::ms(delay);
    //
    //        mcu::pc_13::low();
    //        ::delay::ms(delay);
    //    }
    while (1) {
    }
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler<usart_01_tx_buffer>();
}

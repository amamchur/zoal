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
#include <zoal/utils/vt100.hpp>

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

zoal::io::button<tools, mcu::pb_13> user_button_1;
zoal::io::button<tools, mcu::pb_12> user_button_2;

[[noreturn]] void zoal_input_handler(void *) {
    while (1) {
        auto events = user_button_1.handle();
        if ((events & zoal::io::button_state_trigger_press) != 0) {
            logger::stream() << zoal::utils::vt100::ris();
        }

        events = user_button_2.handle();
        if ((events & zoal::io::button_state_trigger_press) != 0) {
            mcu::pc_13::toggle();
        }

        osDelay(1);
    }
}

extern "C" void zoal_init() {
    api::optimize<api::power_on<usart_01, mcu::port_a, mcu::port_c, mcu::port_b>>();
    api::optimize<api::disable<usart_01>>();

    api::optimize<
        //
        mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::connect,
        mcu::cfg::usart<usart_01, 115200>::cfg,
        //
        api::mode<zoal::gpio::pin_mode::input_pull_up, mcu::pb_12, mcu::pb_13>,
        api::mode<zoal::gpio::pin_mode::output, mcu::pc_13>
        //
        >();

    api::optimize<api::enable<usart_01>>();

    NVIC_EnableIRQ(USART1_IRQn);

    inputTask_attributes.name = "inputHandler";
    inputTask_attributes.priority = osPriorityLow;
    inputTask_attributes.stack_size = 64 * 4;
    inputTaskHandle = osThreadNew(zoal_input_handler, NULL, &inputTask_attributes);
}

static void vt100_print() {
    using namespace zoal::utils;
    using zoal::metadata::signal;
    using zoal::metadata::stm32_remap;

    using pa09 = mcu::pa_09;
    using pa09r = stm32_remap<usart_01::address, pa09::port::address, pa09::offset, signal::tx>;

    using pa10 = mcu::pa_10;
    using pa10r = stm32_remap<usart_01::address, pa10::port::address, pa10::offset, signal::rx>;

    auto stream = logger::stream();
    stream << vt100::ris();
    stream << "PA09 TX AF: " << pa10r::value << vt100::cr_lf();
    stream << "PA10 RX AF: " << pa10r::value << vt100::cr_lf();

    zoal::utils::cas_print_functor<logger> func;

    logger::trace() << "rx_list";
    using rx_list = mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::rx_in;
    zoal::ct::type_list_iterator<rx_list>::for_each(func);

    logger::trace() << "tx_list";
    using tx_list = mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::tx_af;
    zoal::ct::type_list_iterator<tx_list>::for_each(func);

    logger::trace() << "connect";
    using connect = mcu::mux::usart<usart_01, mcu::pa_10, mcu::pa_09>::connect;
    zoal::ct::type_list_iterator<connect>::for_each(func);

    logger::info() << "GPIOA_CRL: *" << (void *)mcu::port_a::GPIOx_CRL::address << "=" << (void *)mcu::port_a::GPIOx_CRL::ref();
    logger::info() << "GPIOA_CRH: *" << (void *)mcu::port_a::GPIOx_CRH::address << "=" << (void *)mcu::port_a::GPIOx_CRH::ref();
}

extern "C" [[noreturn]] void zoal_default_thread(void *argument) {
    zoal_init();

    vt100_print();

    constexpr int delay = 3000;
    for (;;) {
        //        logger::info() << "Heartbeat: " << osKernelGetTickCount();
        ::delay::ms(delay);
    }
}

extern "C" void USART1_IRQHandler() {
    usart_01::tx_handler<usart_01_tx_buffer>();
}

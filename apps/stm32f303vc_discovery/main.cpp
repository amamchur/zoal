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
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/new.hpp>

using mcu = zoal::mcu::stm32f303vctx;
using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using i2c_01 = mcu::i2c_01;

zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> io_events;

[[noreturn]] void zoal_main_task(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
__unused zoal::mem::reserve_mem<task_type, 512, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> terminal_task(zoal_terminal_rx_task, "terminal");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");

zoal::utils::i2c_scanner scanner;

static void test_i2c() {
    vTaskDelay(1000);
    scanner.device_found = [](uint8_t addr) {
        tx_stream << "\033[2K\r"
                  << "I2C Device found: " << zoal::io::hexadecimal(addr) << "\r\n";
    };
    scanner.scan(i2c_dispatcher)([](int code) {
        if (code == 0) {
            tx_stream << "Done\r\n";
        } else {
            tx_stream << "Error\r\n";
        }
        terminal.sync();
    });
}

class functor {
public:
    template<class T>
    void operator()() {
        using cas = typename T::type;
        auto reg = reinterpret_cast<uint32_t *>(cas::address);
        tx_stream << "ADDR: " << zoal::io::hexadecimal(cas::address) << " " << zoal::io::hexadecimal(cas::clear) << " " << zoal::io::hexadecimal(cas::set);
        tx_stream << " REG: " << zoal::io::hexadecimal(*reg) << "\r\n";
    }
};

[[noreturn]] void zoal_main_task(void *) {
    using api = zoal::gpio::api;
    using usart_speed_cfg = zoal::periph::usart_115200<36000000>;
    using usart_mux = mcu::mux::usart<debug_usart, mcu::pb_11, mcu::pb_10>;
    using usart_cfg = mcu::cfg::usart<debug_usart, usart_speed_cfg>;

    vTaskDelay(100);


    functor fn;
    tx_stream << "\r\n";
    zoal::ct::type_chain_iterator<usart_cfg::apply>::for_each(fn);

    for (;;) {
        auto bits = io_events.wait(1);
        if (bits) {
            i2c_dispatcher.handle();
        }

        vTaskDelay(1);
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

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

extern "C" void I2C1_EV_IRQHandler() {
    i2c_01::handle_request_irq(request, []() { io_events.set_isr(1); });
}

extern "C" void I2C1_ER_IRQHandler() {
    i2c_01::handle_request_irq(request, []() { io_events.set_isr(1); });
}

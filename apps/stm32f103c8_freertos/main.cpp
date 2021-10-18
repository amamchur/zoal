#include "command_processor.hpp"
#include "command_queue.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "input_processor.hpp"
#include "stm32f1xx_hal.h"
#include "terminal.hpp"

#include <zoal/freertos/task.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/periph/timer.hpp>

[[noreturn]] void zoal_main_task(void *);

using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<72000000, ms_counter>;
using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;

__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> input_task(zoal_input_processor, "input");
__attribute__((unused)) __unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

class functor {
public:
    static void print_cas(uintptr_t addr, uint32_t c, uint32_t s) {
        tx_stream << "A: " << zoal::io::hexadecimal(addr);
        tx_stream << " C: " << zoal::io::hexadecimal(c);
        tx_stream << " S: " << zoal::io::hexadecimal(s);
        tx_stream << "\r\n";
    }

    template<class T>
    void operator()() {
        using cas = typename T::type;
        print_cas(cas::address, cas::clear, cas::set);
    }
};

void test_timer() {
    using zoal::gpio::api;
    using timer = mcu::timer_02;
    using timer_cfg = zoal::periph::timer_config<72000000, 36000, 0xFFFF, zoal::periph::timer_mode::up>;
    using timer_cfg_cas = mcu::cfg::timer<timer, timer_cfg>::apply;

    functor fn;
    zoal::ct::type_chain_iterator<timer_cfg_cas>::for_each(fn);

    api::optimize<timer::clock_on_cas>();
    api::optimize<timer_cfg_cas>();
    api::optimize<timer::enable_cas>();
}

[[noreturn]] void zoal_main_task(void *) {
    init_terminal();

    test_timer();

    for (;;) {
        auto bits = hardware_events.wait(all_hardware_events);
        if ((bits & i2c_event) == i2c_event) {
            i2c_dispatcher.handle();
        }

        if ((bits & usart_event) == usart_event) {
            uint8_t rx_buffer[8];
            size_t size;
            do {
                size = rx_stream_buffer.receive(rx_buffer, sizeof(rx_buffer), 0);
                if (size != 0) {
                    terminal.push_and_scan(rx_buffer, size);
                }
            } while (size == sizeof(rx_buffer));
        }
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

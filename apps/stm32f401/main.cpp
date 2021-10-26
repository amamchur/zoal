#include "adc.h"
#include "command_processor.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include "terminal.hpp"
#include "tim.h"
#include "usart.h"
#include "w25qxx.h"

#include <cstring>
#include <zoal/data/ring_buffer.hpp>
#include <zoal/freertos/task.hpp>

using mcu = zoal::mcu::stm32f401ccux;
using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::cmsis_os2::delay<84000000>;
using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
using api = zoal::gpio::api;
using spi = mcu::spi_01;

[[noreturn]] void zoal_main_task(void *);

__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

using w25q32 = zoal::ic::w25qxx<spi, mcu::pa_04, delay>;

uint8_t test_buffer[16] = {0};

class funct {
public:
    static void print_cas(uint32_t addr, uint32_t c, uint32_t s) {
        using zoal::io::hexadecimal;
        tx_stream << "A: " << hexadecimal(addr) << " C: " << hexadecimal(c) << " S: " << hexadecimal(s) << "\r\n";
    }

    template<class T>
    void operator()() {
        using cas = typename T::type;
        print_cas(cas::address, cas::clear, cas::set);
    }
};

[[noreturn]] void zoal_main_task(void *) {
    init_terminal();

    W25qxx_Init();
    memset(test_buffer, 0, sizeof(test_buffer));
    W25qxx_ReadBytes(test_buffer, 0, sizeof(test_buffer));

    tx_stream << "\r\n";

    for (unsigned char i : test_buffer) {
        tx_stream << zoal::io::hexadecimal(i) << " ";
    }
    tx_stream << "\r\n";

    memset(test_buffer, 0, sizeof(test_buffer));
    w25q32::read_data(0, test_buffer, sizeof(test_buffer));
    for (unsigned char i : test_buffer) {
        tx_stream << zoal::io::hexadecimal(i) << " ";
    }
    tx_stream << "\r\n";

    terminal.sync();

    for (;;) {
        auto bits = hardware_events.wait(all_hardware_events);
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

using timer = mcu::timer_02;
using pin = mcu::pa_00;
using ch = mcu::mux::pwm_channel<timer, pin>;

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    MX_SPI1_Init();
    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

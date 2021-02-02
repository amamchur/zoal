#include "../misc/at24cxx.hpp"
#include "command_processor.hpp"
#include "command_queue.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "i2c.h"
#include "input_processor.hpp"
#include "rtc.h"
#include "stm32f1xx_hal.h"
#include "terminal.hpp"

#include <cstring>
#include <zoal/arch/cortex/stm32f1/i2c.hpp>
#include <zoal/freertos/task.hpp>
#include <zoal/freertos/event_group.hpp>
#include <zoal/ic/ds3231.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/delay.hpp>
#include <zoal/utils/i2c_scanner.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/new.hpp>

using ms_counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using delay = zoal::utils::delay<72000000, ms_counter>;

[[noreturn]] void zoal_main_task(void *);

using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__unused zoal::mem::reserve_mem<task_type, 128, StackType_t> input_task(zoal_input_processor, "input");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> terminal_task(zoal_terminal_rx_task, "terminal");
__unused zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");
zoal::freertos::event_group<zoal::freertos::freertos_allocation_type::static_mem> i2c_02_events;

using i2c_01 = zoal::arch::stm32f1::i2c<0x40005400>;
using i2c_02 = zoal::arch::stm32f1::i2c<0x40005800>;
using i2c_req_dispatcher_type = zoal::periph::i2c_request_dispatcher<i2c_02, sizeof(void *) * 8>;
i2c_req_dispatcher_type i2c_req_dispatcher;
zoal::periph::i2c_request &request = i2c_req_dispatcher.request;

uint8_t i2c_shared_buffer[32];
auto constexpr buffer_mem_type = zoal::periph::device_buffer_type::shared_mem;
zoal::ic::ds3231<buffer_mem_type> clock(i2c_shared_buffer);
zoal::ic::at24cxx<buffer_mem_type> eeprom(i2c_shared_buffer);
zoal::utils::i2c_scanner scanner;

static void read_clock() {
    clock.fetch(i2c_req_dispatcher)([](int) {
        auto dt = clock.date_time();
        tx_stream << "\033[2K\rDate time: " << "\r\n" << dt << "\r\n";
        terminal.sync();
    });
}

uint8_t eeprom_mem[32];

static void read_eeprom() {
    eeprom.read(i2c_req_dispatcher, 0, eeprom_mem, 4)([](int code) {
        if (code == 0) {
            tx_stream << "\033[2K\r"
                      << "EEPROM:\r\n";
            for (int i = 0; i < 16; i++) {
                tx_stream << zoal::io::hexadecimal(eeprom_mem[i]) << "\r\n";
            }
        } else {
            tx_stream << "\033[2K\r"
                      << "Error"
                      << "\r\n";
        }
        terminal.sync();
        read_clock();
    });
}

static void test_i2c() {
    vTaskDelay(100);

    scanner.device_found = [](uint8_t addr) {
        tx_stream << "\033[2K\r"
                  << "I2C Device found: " << zoal::io::hexadecimal(addr) << "\r\n";
    };
    scanner.scan(i2c_req_dispatcher)([](int code) {
        if (code == 0) {
            tx_stream << "Done\r\n";
            read_eeprom();
        } else {
            tx_stream << "Error\r\n";
        }
        terminal.sync();
    });
}

[[noreturn]] void zoal_main_task(void *) {
    test_i2c();

    for (;;) {
        auto bits = i2c_02_events.wait(1);
        if (bits) {
            i2c_req_dispatcher.handle();
        }
    }
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_RTC_Init();
    MX_I2C2_Init();
    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

extern "C" void I2C2_EV_IRQHandler(void) {
    i2c_02::handle_request_irq(request, []() {
        i2c_02_events.set_isr(1);
    });
}

extern "C" void I2C2_ER_IRQHandler(void) {
    i2c_02::handle_request_irq(request, []() {
        i2c_02_events.set_isr(1);
    });
}

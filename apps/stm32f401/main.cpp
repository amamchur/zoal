#include "../data/roboto_regular_16.hpp"
#include "adc.h"
#include "command_processor.hpp"
#include "gpio.h"
#include "hardware.hpp"
#include "i2c.h"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include "terminal.hpp"
#include "tim.h"
#include "usart.h"
#include "w25qxx.h"

#include <zoal/data/ring_buffer.hpp>
#include <zoal/freertos/task.hpp>

using delay = zoal::utils::cmsis_os2::delay<84000000>;
using task_type = zoal::freertos::task<zoal::freertos::freertos_allocation_type::static_mem>;

[[noreturn]] void zoal_main_task(void *);

void test_ssh1106();
void write_flash();
__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> command_task(zoal_cmd_processor, "command");
__attribute__((unused)) zoal::mem::reserve_mem<task_type, 256, StackType_t> main_task(zoal_main_task, "main");

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

void print_i2c(I2C_TypeDef *i2c) {
    tx_stream << "CR1\t" << (void *)&i2c->CR1 << "\t" << zoal::io::hexadecimal(i2c->CR1) << "\r\n";
    tx_stream << "CR2\t" << (void *)&i2c->CR2 << "\t" << zoal::io::hexadecimal(i2c->CR2) << "\r\n";
    tx_stream << "OAR1\t" << (void *)&i2c->OAR1 << "\t" << zoal::io::hexadecimal(i2c->OAR1) << "\r\n";
    tx_stream << "OAR2\t" << (void *)&i2c->OAR2 << "\t" << zoal::io::hexadecimal(i2c->OAR2) << "\r\n";
    tx_stream << "CCR\t" << (void *)&i2c->CCR << "\t" << zoal::io::hexadecimal(i2c->CCR) << "\r\n";
    tx_stream << "TRISE\t" << (void *)&i2c->TRISE << "\t" << zoal::io::hexadecimal(i2c->TRISE) << "\r\n";
}

uint8_t test_buffer[16];

[[noreturn]] void zoal_main_task(void *) {
    init_terminal();

    terminal.sync();

#if 0
    using i2c_01_params = zoal::periph::i2c_fast_mode<42000000>;
    using i2c_mux = mcu::mux::i2c<mcu::i2c_01, mcu::pb_09, mcu::pb_08>;
    using i2c_cfg = mcu::cfg::i2c<mcu::i2c_01, i2c_01_params>;

    tx_stream << "\r\nConfig: \r\n";

    funct fn;
    zoal::ct::type_chain_iterator<i2c_cfg::apply>::for_each(fn);

    //    I2C_TypeDef *i2c = I2C1;
    tx_stream << "\r\nPrev I2C1: \r\n";
    print_i2c(&prev_i2c);
    tx_stream << "\r\nCurrent I2C1: \r\n";
    print_i2c(I2C1);

    tx_stream << "Mux: \r\n";
    zoal::ct::type_chain_iterator<i2c_mux::connect>::for_each(fn);
#endif

    //    W25qxx_Init();
    //    memset(test_buffer, 0, sizeof(test_buffer));
    //    W25qxx_ReadBytes(test_buffer, 0, sizeof(test_buffer));
    //
    //    tx_stream << "!!! w25qxx: " << (int)w25qxx.ID << "\r\n";
    //    tx_stream << "\r\n";

    for (unsigned char i : test_buffer) {
        tx_stream << zoal::io::hexadecimal(i) << " ";
    }

    test_ssh1106();
    //    write_flash();

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

#include <zoal/gfx/glyph_renderer.hpp>
#include <zoal/gfx/renderer.hpp>
#include <zoal/ic/sh1106.hpp>

using adapter = zoal::ic::sh1106_adapter_0<128, 64>;
using graphics = zoal::gfx::renderer<uint8_t, adapter>;
using oled_type = zoal::ic::sh1106_spi<128, 64, oled_spi, oled_res, oled_ds, oled_cs, delay>;

oled_type screen;

class mem_reader {
public:
    template<class T>
    static T read_mem(const void *ptr) {
        T t;
        memcpy(&t, ptr, sizeof(t));
        return t;
    }
};

void test_ssh1106() {
    screen.init();

    auto g = graphics::from_memory(screen.buffer.canvas);
    zoal::gfx::glyph_renderer<graphics, mem_reader> gl(g, &roboto_regular_16);

    w25q32::read(0, &screen.buffer.canvas, sizeof(screen.buffer.canvas));

    screen.display();
}

extern "C" void SystemClock_Config(void);

int main() {
    HAL_Init();

    SystemClock_Config();

    zoal_init_hardware();

    vTaskStartScheduler();
    return 0;
}

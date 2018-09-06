#include "stm32f30x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zoal/board/nucleo_f303re.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/ic/ws2812.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/shields/uno_lcd_shield.hpp>
#include <zoal/arch/cortex/stm32f3/adc.hpp>
#include <zoal/arch/cortex/stm32x/clock_control.hpp>
#include <zoal/arch/cortex/semihosting_transport.hpp>
#include <zoal/periph/adc_connection.hpp>
#include <zoal/mcu/atmega_640_1280_2560.hpp>
#include <zoal/mcu/stm32f1xx.hpp>

#include "templates/multi_function_shield.hpp"
#include "templates/ir_remove.hpp"
#include "templates/compile_check.hpp"

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::pcb::mcu;
using usart = mcu::usart1<32, 32>;
using logger = zoal::utils::plain_logger<usart, zoal::utils::log_level::info>;
using counter = zoal::utils::ms_counter<uint32_t, &milliseconds_counter>;
using tools = zoal::utils::tool_set<mcu, counter, logger>;
using delay = typename tools::delay;
using connection = mcu::pa00_adc1;
using shield = zoal::shields::uno_lcd_shield<tools, zoal::pcb>;
using lcd_output_stream = zoal::io::output_stream<shield::lcd>;

using app1 = multi_function_shield<tools, zoal::pcb>;
using check = compile_check<app1>;

app1 app;
lcd_output_stream stream;

#pragma GCC diagnostic push

void handler(uint8_t button, zoal::io::button_event e) {
    if (e != zoal::io::button_event::press) {
        return;
    }

    stream << zoal::io::pos(1, 0) << "Button: " << button;
}

void initTimer() {
    zoal::pcb::build_in_led::port::power_on();
    zoal::pcb::build_in_led::mode<zoal::gpio::pin_mode::output>();

    mcu::timer2::power_on();
    mcu::timer2::prescaler<7200>();
    mcu::timer2::period<5000>();
    mcu::timer2::enable_interrupt<zoal::periph::timer_interrupt::overflow>();
    mcu::timer2::enable();
}

#define RINGBUF_SIZE_BITS 5
#define RINGBUF_SIZE (1<<RINGBUF_SIZE_BITS)

volatile uint8_t USART1_ringbuf[RINGBUF_SIZE];
volatile uint32_t USART1_readidx = 0;
volatile uint32_t USART1_writeidx = 0;

void USART1_Init() {
    usart::power_on();

    mcu::mux::usart<usart, mcu::pa09, mcu::pa10, mcu::pa08>::on();

    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 57600;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    NVIC_EnableIRQ(USART1_IRQn);
}

#define MIN(a, b) ((a)<(b)?(a):(b))

void USART1_putc(char ch) {
    while (1) {
        uint32_t capacity = RINGBUF_SIZE - (USART1_writeidx - USART1_readidx);
        if (capacity > 0)
            break;
    }
    USART1_ringbuf[(USART1_writeidx++) & (RINGBUF_SIZE - 1)] = ch;
}

void USART1_write(const char *str, int len) {
    uint32_t i = 0;
    while (i < len) {
        uint32_t writeidx = USART1_writeidx & (RINGBUF_SIZE - 1);
        uint32_t len_to_end = RINGBUF_SIZE - writeidx;
        uint32_t capacity = RINGBUF_SIZE - (USART1_writeidx - USART1_readidx);
        uint32_t max_len = MIN(len_to_end, capacity);
        if (max_len == 0)
            continue;

        uint32_t this_len = MIN(max_len, len - i);

        int j;
        for (j = 0; j < this_len; ++j) {
            USART1_ringbuf[writeidx++] = str[i++];
        }
        USART1_writeidx += this_len;

        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
}

void USART1_print(const char *str) {
    uint32_t len = strlen(str);
    USART1_write(str, len);
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);
    zoal::utils::interrupts::on();

    USART1_Init();

//    initTimer();
//
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    NVIC_EnableIRQ(TIM2_IRQn);

//    logger::info() << "logger::info";

    usart::write('A');
    usart::write('A');
    usart::write('A');
    usart::write('\r');
    usart::write('\n');
//    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//    USART1_print("USART1_print\n\n");

    while (1) {
//        USART1_print("USART1_print\n\n");
        logger::info() << "Hello logger";
        delay::ms(1000);
    }

    return 0;
}

#pragma GCC diagnostic pop

#if 0

extern "C" void USART1_EXTI25_IRQHandler(void) {
    if (USART1_writeidx - USART1_readidx == 0) {
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
        return;
    }
    USART_SendData(USART1,
                   USART1_ringbuf[(USART1_readidx++) & (RINGBUF_SIZE - 1)]);
}

#else

extern "C" void USART1_EXTI25_IRQHandler(void) {
    usart::handleIrq();
}

#endif

extern "C" void SysTick_Handler() {
    milliseconds_counter++;
}

extern "C" void TIM2_IRQHandler() {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    zoal::pcb::build_in_led::toggle();
}

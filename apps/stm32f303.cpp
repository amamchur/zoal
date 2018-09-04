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

#include "templates/multi_function_shield.hpp"
#include "templates/ir_remove.hpp"
#include "templates/compile_check.hpp"

volatile uint32_t milliseconds_counter = 0;

using mcu = zoal::pcb::mcu;
using logger = zoal::utils::plain_logger<void, zoal::utils::log_level::info>;
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

void initAll() {
    zoal::pcb::build_in_led::port::power_on();
    zoal::pcb::build_in_led::mode<zoal::gpio::pin_mode::output>();

    mcu::timer2::power_on();
    mcu::timer2::prescaler<7200>();
    mcu::timer2::period<5000>();
    mcu::timer2::enable_interrupt<zoal::periph::timer_interrupt::overflow>();
    mcu::timer2::enable();
}

int main() {
    SysTick_Config(SystemCoreClock / 1000);
    zoal::utils::interrupts::on();

    initAll();

//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    TIM_Cmd(TIM2, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);

    while (1) {
    }

    return 0;
}

#pragma GCC diagnostic pop


extern "C" void SysTick_Handler() {
    milliseconds_counter++;
}

extern "C" void TIM2_IRQHandler() {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    zoal::pcb::build_in_led::toggle();
}

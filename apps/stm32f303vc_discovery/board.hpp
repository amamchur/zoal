#ifndef ZOAL_STM32F303_FREERTOS_BOARD_HPP
#define ZOAL_STM32F303_FREERTOS_BOARD_HPP

#include <zoal/board/discovery_stm32f303vc.hpp>
#include <zoal/mcu/stm32f303vctx.hpp>

using pcb = zoal::board::discovery_stm32f303vc;
using mcu = pcb::mcu;
using tty_usart = mcu::usart_03;

#endif

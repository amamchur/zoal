#ifndef ZOAL_STM32F103C8_FREERTOS_BOARD_HPP
#define ZOAL_STM32F103C8_FREERTOS_BOARD_HPP

#include <zoal/mcu/stm32f103c8tx.hpp>
#include <zoal/utils/cmsis_os2/tool_set.hpp>

using mcu = zoal::mcu::stm32f103c8tx;
using usart_01 = mcu::usart_01;
using user_led = mcu::pc_13;

#endif

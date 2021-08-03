#ifndef ZOAL_STM32F103C8_FREERTOS_BOARD_HPP
#define ZOAL_STM32F103C8_FREERTOS_BOARD_HPP

#include <zoal/mcu/stm32f103c8tx.hpp>

using mcu = zoal::mcu::stm32f103c8tx;
using tty_usart = mcu::usart_01;
using i2c_02 = mcu::i2c_02;
using user_led = zoal::gpio::active_drain<mcu::pc_13>;

#endif

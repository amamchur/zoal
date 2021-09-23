#ifndef ZOAL_HARDWARE_HPP
#define ZOAL_HARDWARE_HPP

#include "stm32f0xx_hal.h"

#include <zoal/data/ring_buffer.hpp>
#include <zoal/mcu/stm32f030f4px.hpp>
#include <zoal/utils/ms_counter.hpp>
#include <zoal/utils/tool_set.hpp>
#include <zoal/utils/usart_transmitter.hpp>

using mcu = zoal::mcu::stm32f030f4px;
using led_pin = mcu::pa_04;
using usart = mcu::usart_01;
using usart_speed_cfg = zoal::periph::usart_115200<48000000>;
using usart_mux = mcu::mux::usart<usart, mcu::pa_03, mcu::pa_02>;
using usart_cfg = mcu::cfg::usart<usart, usart_speed_cfg>;

using counter = zoal::utils::ms_counter<uint32_t, &uwTick>;
using tools = zoal::utils::tool_set<mcu, 48000000, counter, void>;
using delay = tools::delay;

using tx_buffer_type = zoal::data::ring_buffer<uint8_t, 32>;
using rx_buffer_type = zoal::data::ring_buffer<uint8_t, 32>;
using usart_tx_type = zoal::utils::usart_transmitter<usart, tx_buffer_type>;
using usart_tx_stream_type = zoal::io::output_stream<usart_tx_type>;

extern tx_buffer_type tx_buffer;
extern rx_buffer_type rx_buffer;
extern usart_tx_type usart_tx;
extern usart_tx_stream_type stream;

#endif

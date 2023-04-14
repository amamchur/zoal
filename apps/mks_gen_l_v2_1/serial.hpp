#pragma once

#ifndef ZOAL_SERIAL_HPP
#define ZOAL_SERIAL_HPP

#include "pcb.hpp"

#include <zoal/arch/avr/utils/usart_transmitter.hpp>
#include <zoal/utils/logger.hpp>

using usart = mcu::usart_00;
using usart_tx_type = zoal::utils::usart_transmitter<usart, 32, zoal::utils::interrupts_off>;
using usart_tx_stream_type = zoal::io::output_stream<usart_tx_type>;

extern zoal::data::ring_buffer<uint8_t, 256> rx_buffer;
extern usart_tx_type usart_tx;
extern usart_tx_stream_type stream;

#endif

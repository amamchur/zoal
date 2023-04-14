#pragma once

#ifndef ZOAL_RAMPS_HPP
#define ZOAL_RAMPS_HPP

#include "pcb.hpp"
#include "ms_timer.hpp"
#include <zoal/ic/hd44780.hpp>
#include <zoal/io/rotary_encoder.hpp>
#include <zoal/io/button.hpp>

using ramps_exp1_interface = zoal::ic::hd44780_interface_4bit<
    delay,
    pcb::ard_d16,
    pcb::ard_d17,
    pcb::ard_d23,
    pcb::ard_d25,
    pcb::ard_d27,
    pcb::ard_d29>;
using lcd_address_selector = zoal::ic::hd44780_address_selector<20, 4>;
using lcd_type = zoal::ic::hd44780<ramps_exp1_interface, lcd_address_selector>;
using beep = zoal::gpio::active_high<pcb::ard_d37>;

using encoder_type = zoal::io::rotary_encoder<pcb::ard_d31, pcb::ard_d33, zoal::io::rotary_4phase_machine>;
using enc_btn_type = zoal::io::button<uint32_t, pcb::ard_d35>;

using sd_miso = pcb::ard_d50;
using sd_mosi = pcb::ard_d51;
using sd_sck = pcb::ard_d52;
using sd_ss = pcb::ard_d53;
using sd_detection = pcb::ard_d49;

#endif

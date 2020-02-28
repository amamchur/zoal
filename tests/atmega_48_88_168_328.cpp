#include "utils/accessor.hpp"

#include "gtest/gtest.h"
#include <cstring>
#include <iostream>
#include <zoal/mcu/atmega328p.hpp>
#include <zoal/utils/helpers.hpp>

using namespace zoal::gpio;
using mcu = zoal::mcu::atmega328p<16000000>;
using mcu_memory_type = tests::utils::mcu_memory<tests::utils::ATmegaMemory>;

class ATmegaxx8 : public ::testing::Test {
protected:
    using seg = zoal::mem::avr_plain;

    void SetUp() override {
        seg::memory = reinterpret_cast<uint8_t *>(&mem.memory);
    }

    void TearDown() override {
        seg::memory = nullptr;
    }

    mcu_memory_type mem;
};

TEST_F(ATmegaxx8, PIO_NULL_PIN) {
    null_pin::low();
    null_pin::high();
    null_pin::toggle();
    null_pin::write(1);
    null_pin::write(0);
    null_pin::set<0>();
    null_pin::set<1>();

    auto result = mem.equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mem.error, "");
}

TEST_F(ATmegaxx8, GPIO_PIN) {
    mcu::pb_00::mode<pin_mode::output>();
    mcu::pb_01::mode<pin_mode::output>();
    mcu::pb_02::mode<pin_mode::output>();
    mcu::pb_03::mode<pin_mode::output>();
    mcu::pb_04::mode<pin_mode::output>();
    mcu::pb_05::mode<pin_mode::output>();
    mcu::pb_06::mode<pin_mode::output>();

    mcu::pb_00::low();
    mcu::pb_01::high();
    mcu::pb_02::toggle();
    mcu::pb_03::write(1);
    mcu::pb_04::write(0);
    mcu::pb_05::set<0>();
    mcu::pb_06::set<1>();

    //    McuMemory::save("data/ATmegaxx8_GPIO_PIN_0.mem");
    auto result = mem.equal("data/ATmegaxx8_GPIO_PIN_0.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mem.error, "");

    mcu::pb_03::write(0);
    mcu::pb_04::write(1);
    mcu::pb_05::set<1>();
    mcu::pb_06::set<0>();

    //    McuMemory::save("data/ATmegaxx8_GPIO_PIN_1.mem");
    result = mem.equal("data/ATmegaxx8_GPIO_PIN_1.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mem.error, "");
}

TEST_F(ATmegaxx8, GPIO_xxx) {
    mcu::port_b::low<1>();
}

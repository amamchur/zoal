#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <chrono>

#include "./utils/McuMemory.hpp"
#include "./utils/NoopCycles.hpp"

#define ZOAL_UTILS_MEMORY_SEGMENT_HPP

typedef Tests::Utils::McuMemory<Tests::Utils::ATmegaMemory> McuMemory;
namespace zoal {
    namespace utils {
        template<class RegType, uintptr_t base>
        class memory_segment : public McuMemory::MemorySegment<RegType, base> {
        };
    }
}

#include <zoal/mcu/atmega_48_88_168_328.hpp>
#include <zoal/utils/helpers.hpp>

using namespace zoal::gpio;
using namespace zoal::mcu;

typedef atmega_48_88_168_328<16000000> MCU;
typedef Tests::Utils::NoopCycles<16000000, 0> NCT;
typedef Tests::Utils::RegisterAccessor<uint8_t> RegisterAccessor;

MCU::gpio_chain allPins() {
    return MCU::gpio_chain()
           & MCU::pb0()
           & MCU::pb1()
           & MCU::pb2()
           & MCU::pb3()
           & MCU::pb4()
           & MCU::pb5()
           & MCU::pb6()
           & MCU::pb7()
           & MCU::pc0()
           & MCU::pc1()
           & MCU::pc2()
           & MCU::pc3()
           & MCU::pc4()
           & MCU::pc5()
           & MCU::pc6()
           & MCU::pd0()
           & MCU::pd1()
           & MCU::pd2()
           & MCU::pd3()
           & MCU::pd4()
           & MCU::pd5()
           & MCU::pd6()
           & MCU::pd7();
}

MCU::gpio_chain allPinsOddEven() {
    return MCU::gpio_chain()
           & MCU::pb0(0)
           & MCU::pb1(1)
           & MCU::pb2(0)
           & MCU::pb3(1)
           & MCU::pb4(0)
           & MCU::pb5(1)
           & MCU::pb6(0)
           & MCU::pb7(1)
           & MCU::pc0(0)
           & MCU::pc1(1)
           & MCU::pc2(0)
           & MCU::pc3(1)
           & MCU::pc4(0)
           & MCU::pc5(1)
           & MCU::pc6(0)
           & MCU::pd0(0)
           & MCU::pd1(1)
           & MCU::pd2(0)
           & MCU::pd3(1)
           & MCU::pd4(0)
           & MCU::pd5(1)
           & MCU::pd6(0)
           & MCU::pd7(1);
}

class ATmegaxx8 : public ::testing::Test {
protected:
    void SetUp() {
        RegisterAccessor::resetCallbacks();
        McuMemory::clear();
    }
};

TEST_F(ATmegaxx8, GPIO_xxx) {
    MCU::pb0::read();
}

#if 1

TEST_F(ATmegaxx8, GPIO_NULL_PIN) {

    null_pin::low();
    null_pin::high();
    null_pin::toggle();
    null_pin::write(1);
    null_pin::write(0);
    null_pin::set<0>();
    null_pin::set<1>();

    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_PIN) {

    MCU::pb0::mode<pin_mode::output>();
    MCU::pb1::mode<pin_mode::output>();
    MCU::pb2::mode<pin_mode::output>();
    MCU::pb3::mode<pin_mode::output>();
    MCU::pb4::mode<pin_mode::output>();
    MCU::pb5::mode<pin_mode::output>();
    MCU::pb6::mode<pin_mode::output>();

    MCU::pb0::low();
    MCU::pb1::high();
    MCU::pb2::toggle();
    MCU::pb3::write(1);
    MCU::pb4::write(0);
    MCU::pb5::set<0>();
    MCU::pb6::set<1>();

//    McuMemory::save("data/ATmegaxx8_GPIO_PIN_0.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_PIN_0.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::pb3::write(0);
    MCU::pb4::write(1);
    MCU::pb5::set<1>();
    MCU::pb6::set<0>();

//    McuMemory::save("data/ATmegaxx8_GPIO_PIN_1.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_PIN_1.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_OUTPUT) {  /* NOLINT */
    using namespace zoal::gpio;

    NCT::measureOverhead();

    MCU::api::mode<pin_mode::output>(MCU::gpio_chain() & null_pin());
    MCU::api::mode<pin_mode::output>(allPins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_OUTPUT.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_OUTPUT.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}


TEST_F(ATmegaxx8, GPIO_ALL_INPUT) {  /* NOLINT */
    using namespace zoal::gpio;

    MCU::api::mode<pin_mode::input>(MCU::gpio_chain() & null_pin());
    MCU::api::mode<pin_mode::input>(allPins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_INPUT.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_INPUT.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_INPUT_PULLUP) {  /* NOLINT */
    using namespace zoal::gpio;

    MCU::api::mode<pin_mode::input_pull_up>(MCU::gpio_chain() & null_pin());
    MCU::api::mode<pin_mode::input_pull_up>(allPins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_INPUT_PULLUP.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_INPUT_PULLUP.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_LOW) {  /* NOLINT */
    using namespace zoal::gpio;

    MCU::api::mode<pin_mode::output>(allPins());
    MCU::api::low(allPins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_LOW.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_HIGH) {  /* NOLINT */
    using namespace zoal::gpio;

    MCU::api::mode<pin_mode::output>(allPins());
    MCU::api::high(allPins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_HIGH.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_TOGGLE) {  /* NOLINT */
    using namespace zoal::gpio;

    MCU::api::mode<pin_mode::output>(allPins());
    MCU::api::low(allPins());
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::toggle(allPins());
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}


TEST_F(ATmegaxx8, GPIO_ALL_WRITE) {  /* NOLINT */
    using namespace zoal::gpio;

    MCU::pb0::read();

    MCU::api::mode<pin_mode::output>(allPins());
    MCU::api::write(allPinsOddEven());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE.mem");
    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    McuMemory::clear();
    MCU::api::mode<pin_mode::output>(MCU::gpio_chain()
                                    & MCU::pb0()
                                    & MCU::pb1()
                                    & MCU::pb2()
                                    & MCU::pb3()
                                    & MCU::pb4()
                                    & MCU::pb5()
                                    & MCU::pb6()
                                    & MCU::pb7());
    MCU::api::write(MCU::gpio_chain());
    MCU::api::write(MCU::gpio_chain()
                    & MCU::pb0(1)
                    & MCU::pb1(1)
                    & MCU::pb2(1)
                    & MCU::pb3(1)
                    & MCU::pb4(1)
                    & MCU::pb5(1)
                    & MCU::pb6(1)
                    & MCU::pb7(1));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_B_HIGH.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_B_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::write(MCU::gpio_chain());
    MCU::api::write(MCU::gpio_chain()
                    & MCU::pb0(0)
                    & MCU::pb1(0)
                    & MCU::pb2(0)
                    & MCU::pb3(0)
                    & MCU::pb4(0)
                    & MCU::pb5(0)
                    & MCU::pb6(0)
                    & MCU::pb7(0));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_B_LOW.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_B_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::write(MCU::gpio_chain()
                    & MCU::pc0(1)
                    & MCU::pc1(1)
                    & MCU::pc2(1)
                    & MCU::pc3(1)
                    & MCU::pc4(1)
                    & MCU::pc5(1)
                    & MCU::pc6(1));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_C_HIGH.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_C_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::write(MCU::gpio_chain()
                    & MCU::pc0(0)
                    & MCU::pc1(0)
                    & MCU::pc2(0)
                    & MCU::pc3(0)
                    & MCU::pc4(0)
                    & MCU::pc5(0)
                    & MCU::pc6(0));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_C_LOW.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_C_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::write(MCU::gpio_chain()
                    & MCU::pd0(1)
                    & MCU::pd1(1)
                    & MCU::pd2(1)
                    & MCU::pd3(1)
                    & MCU::pd4(1)
                    & MCU::pd5(1)
                    & MCU::pd6(1)
                    & MCU::pd7(1));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_D_HIGH.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_D_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::write(MCU::gpio_chain());
    MCU::api::write(MCU::gpio_chain()
                    & MCU::pd0(0)
                    & MCU::pd1(0)
                    & MCU::pd2(0)
                    & MCU::pd3(0)
                    & MCU::pd4(0)
                    & MCU::pd5(0)
                    & MCU::pd6(0)
                    & MCU::pd7(0));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_D_LOW.mem");
    result = McuMemory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_D_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ENABLE_DISABLE) {
    {
        auto value = zoal::utils::same_type<null_pin::port, null_port>::value;
        EXPECT_TRUE(value);
    }

    MCU::api::enable(MCU::gpio_chain() & MCU::pb0() & MCU::pc0() & MCU::pd0());
    MCU::api::enable(MCU::gpio_chain() & MCU::pb0() & MCU::pc0());
    MCU::api::enable(MCU::gpio_chain() & MCU::pb0());
    MCU::api::enable(MCU::gpio_chain() & null_pin());

    auto result = McuMemory::equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");

    MCU::api::disable(MCU::gpio_chain() & MCU::pb0() & MCU::pc0() & MCU::pd0());
    MCU::api::disable(MCU::gpio_chain() & MCU::pb0() & MCU::pc0());
    MCU::api::disable(MCU::gpio_chain() & MCU::pb0());
    MCU::api::disable(MCU::gpio_chain());
    MCU::api::disable(MCU::gpio_chain() & null_pin());

    result = McuMemory::equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(McuMemory::error, "");
}

#endif

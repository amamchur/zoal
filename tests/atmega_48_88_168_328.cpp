#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <chrono>

#include "utils/mcu_memory.hpp"
#include "utils/nop.hpp"

#define ZOAL_UTILS_MEMORY_SEGMENT_HPP

using mcu_memory = tests::utils::mcu_memory<tests::utils::ATmegaMemory>;
namespace zoal {
    namespace utils {
        template<class RegType, uintptr_t base>
        class memory_segment : public mcu_memory::memory_segment<RegType, base> {
        };
    }
}

#include <zoal/mcu/atmega_48_88_168_328.hpp>
#include <zoal/utils/helpers.hpp>

using namespace zoal::gpio;
using namespace zoal::mcu;

using mcu = atmega_48_88_168_328<16000000>;
using nop = tests::utils::nop<16000000, 0>;
using register_accessor = tests::utils::register_accessor<uint8_t>;

mcu::port_chain all_pins() {
    return mcu::port_chain()
           & mcu::pb0()
           & mcu::pb1()
           & mcu::pb2()
           & mcu::pb3()
           & mcu::pb4()
           & mcu::pb5()
           & mcu::pb6()
           & mcu::pb7()
           & mcu::pc0()
           & mcu::pc1()
           & mcu::pc2()
           & mcu::pc3()
           & mcu::pc4()
           & mcu::pc5()
           & mcu::pc6()
           & mcu::pd0()
           & mcu::pd1()
           & mcu::pd2()
           & mcu::pd3()
           & mcu::pd4()
           & mcu::pd5()
           & mcu::pd6()
           & mcu::pd7();
}

mcu::port_chain all_pins_odd_even() {
    return mcu::port_chain()
           & mcu::pb0(0)
           & mcu::pb1(1)
           & mcu::pb2(0)
           & mcu::pb3(1)
           & mcu::pb4(0)
           & mcu::pb5(1)
           & mcu::pb6(0)
           & mcu::pb7(1)
           & mcu::pc0(0)
           & mcu::pc1(1)
           & mcu::pc2(0)
           & mcu::pc3(1)
           & mcu::pc4(0)
           & mcu::pc5(1)
           & mcu::pc6(0)
           & mcu::pd0(0)
           & mcu::pd1(1)
           & mcu::pd2(0)
           & mcu::pd3(1)
           & mcu::pd4(0)
           & mcu::pd5(1)
           & mcu::pd6(0)
           & mcu::pd7(1);
}

class ATmegaxx8 : public ::testing::Test {
protected:
    void SetUp() {
        register_accessor::reset_callbacks();
        mcu_memory::clear();
    }
};

TEST_F(ATmegaxx8, GPIO_xxx) {
    mcu::pb0::read();
}

#if 1

TEST_F(ATmegaxx8, PIO_NULL_PIN) {

    null_pin::low();
    null_pin::high();
    null_pin::toggle();
    null_pin::write(1);
    null_pin::write(0);
    null_pin::set<0>();
    null_pin::set<1>();

    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_PIN) {

    mcu::pb0::mode<pin_mode::output>();
    mcu::pb1::mode<pin_mode::output>();
    mcu::pb2::mode<pin_mode::output>();
    mcu::pb3::mode<pin_mode::output>();
    mcu::pb4::mode<pin_mode::output>();
    mcu::pb5::mode<pin_mode::output>();
    mcu::pb6::mode<pin_mode::output>();

    mcu::pb0::low();
    mcu::pb1::high();
    mcu::pb2::toggle();
    mcu::pb3::write(1);
    mcu::pb4::write(0);
    mcu::pb5::set<0>();
    mcu::pb6::set<1>();

//    McuMemory::save("data/ATmegaxx8_GPIO_PIN_0.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_PIN_0.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::pb3::write(0);
    mcu::pb4::write(1);
    mcu::pb5::set<1>();
    mcu::pb6::set<0>();

//    McuMemory::save("data/ATmegaxx8_GPIO_PIN_1.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_PIN_1.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_OUTPUT) {  /* NOLINT */
    using namespace zoal::gpio;

    nop::measure_overhead();

    mcu::api::mode<pin_mode::output>(mcu::port_chain() & null_pin());
    mcu::api::mode<pin_mode::output>(all_pins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_OUTPUT.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_OUTPUT.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}


TEST_F(ATmegaxx8, GPIO_ALL_INPUT) {  /* NOLINT */
    using namespace zoal::gpio;

    mcu::api::mode<pin_mode::input>(mcu::port_chain() & null_pin());
    mcu::api::mode<pin_mode::input>(all_pins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_INPUT.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_INPUT.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_INPUT_PULLUP) {  /* NOLINT */
    using namespace zoal::gpio;

    mcu::api::mode<pin_mode::input_pull_up>(mcu::port_chain() & null_pin());
    mcu::api::mode<pin_mode::input_pull_up>(all_pins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_INPUT_PULLUP.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_INPUT_PULLUP.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_LOW) {  /* NOLINT */
    using namespace zoal::gpio;

    mcu::api::mode<pin_mode::output>(all_pins());
    mcu::api::low(all_pins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_LOW.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_HIGH) {  /* NOLINT */
    using namespace zoal::gpio;

    mcu::api::mode<pin_mode::output>(all_pins());
    mcu::api::high(all_pins());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_HIGH.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ALL_TOGGLE) {  /* NOLINT */
    using namespace zoal::gpio;

    mcu::api::mode<pin_mode::output>(all_pins());
    mcu::api::low(all_pins());
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::toggle(all_pins());
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}


TEST_F(ATmegaxx8, GPIO_ALL_WRITE) {  /* NOLINT */
    using namespace zoal::gpio;

    mcu::pb0::read();

    mcu::api::mode<pin_mode::output>(all_pins());
    mcu::api::write(all_pins_odd_even());

//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE.mem");
    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu_memory::clear();
    mcu::api::mode<pin_mode::output>(mcu::port_chain()
                                     & mcu::pb0()
                                     & mcu::pb1()
                                     & mcu::pb2()
                                     & mcu::pb3()
                                     & mcu::pb4()
                                     & mcu::pb5()
                                     & mcu::pb6()
                                     & mcu::pb7());
    mcu::api::write(mcu::port_chain());
    mcu::api::write(mcu::port_chain()
                    & mcu::pb0(1)
                    & mcu::pb1(1)
                    & mcu::pb2(1)
                    & mcu::pb3(1)
                    & mcu::pb4(1)
                    & mcu::pb5(1)
                    & mcu::pb6(1)
                    & mcu::pb7(1));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_B_HIGH.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_B_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::write(mcu::port_chain());
    mcu::api::write(mcu::port_chain()
                    & mcu::pb0(0)
                    & mcu::pb1(0)
                    & mcu::pb2(0)
                    & mcu::pb3(0)
                    & mcu::pb4(0)
                    & mcu::pb5(0)
                    & mcu::pb6(0)
                    & mcu::pb7(0));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_B_LOW.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_B_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::write(mcu::port_chain()
                    & mcu::pc0(1)
                    & mcu::pc1(1)
                    & mcu::pc2(1)
                    & mcu::pc3(1)
                    & mcu::pc4(1)
                    & mcu::pc5(1)
                    & mcu::pc6(1));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_C_HIGH.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_C_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::write(mcu::port_chain()
                    & mcu::pc0(0)
                    & mcu::pc1(0)
                    & mcu::pc2(0)
                    & mcu::pc3(0)
                    & mcu::pc4(0)
                    & mcu::pc5(0)
                    & mcu::pc6(0));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_C_LOW.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_C_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::write(mcu::port_chain()
                    & mcu::pd0(1)
                    & mcu::pd1(1)
                    & mcu::pd2(1)
                    & mcu::pd3(1)
                    & mcu::pd4(1)
                    & mcu::pd5(1)
                    & mcu::pd6(1)
                    & mcu::pd7(1));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_D_HIGH.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_D_HIGH.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::write(mcu::port_chain());
    mcu::api::write(mcu::port_chain()
                    & mcu::pd0(0)
                    & mcu::pd1(0)
                    & mcu::pd2(0)
                    & mcu::pd3(0)
                    & mcu::pd4(0)
                    & mcu::pd5(0)
                    & mcu::pd6(0)
                    & mcu::pd7(0));
//    McuMemory::save("data/ATmegaxx8_GPIO_ALL_WRITE_D_LOW.mem");
    result = mcu_memory::equal("data/ATmegaxx8_GPIO_ALL_WRITE_D_LOW.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

TEST_F(ATmegaxx8, GPIO_ENABLE_DISABLE) {
    {
        auto value = zoal::utils::is_same<null_pin::port, null_port>::value;
        EXPECT_TRUE(value);
    }

    mcu::api::power_on(mcu::port_chain() & mcu::pb0() & mcu::pc0() & mcu::pd0());
    mcu::api::power_on(mcu::port_chain() & mcu::pb0() & mcu::pc0());
    mcu::api::power_on(mcu::port_chain() & mcu::pb0());
    mcu::api::power_on(mcu::port_chain() & null_pin());

    auto result = mcu_memory::equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");

    mcu::api::power_off(mcu::port_chain() & mcu::pb0() & mcu::pc0() & mcu::pd0());
    mcu::api::power_off(mcu::port_chain() & mcu::pb0() & mcu::pc0());
    mcu::api::power_off(mcu::port_chain() & mcu::pb0());
    mcu::api::power_off(mcu::port_chain());
    mcu::api::power_off(mcu::port_chain() & null_pin());

    result = mcu_memory::equal("data/ATmegaxx8_GPIO_CLEAR.mem");
    EXPECT_TRUE(result);
    EXPECT_EQ(mcu_memory::error, "");
}

#endif

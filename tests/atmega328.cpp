#include "utils/address_cast.hpp"

#include "gtest/gtest.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <zoal/mcu/atmega328p.hpp>

using namespace zoal::gpio;
using mcu = zoal::mcu::atmega328p<16000000>;

class ATmega328 : public ::testing::Test {
protected:
    void SetUp() override {
        zoal::test::mem<void>::clear_mem();
    }

    void TearDown() override {
        zoal::test::mem<void>::clear_mem();
    }
};

TEST_F(ATmega328, port_api_checking) {
    using namespace ::zoal::gpio;
    using port_b = mcu::port_b;
    auto pinb_address = port_b::PINx::address;
    auto ddrb_address = port_b::DDRx::address;
    auto portb_address = port_b::PORTx::address;

    EXPECT_EQ(pinb_address, 0x0023);
    EXPECT_EQ(ddrb_address, 0x0024);
    EXPECT_EQ(portb_address, 0x0025);

    uint8_t *m = zoal::test::mem<void>::avr();

    port_b::high<0xAA>();
    EXPECT_EQ(m[portb_address], 0xAA);

    port_b::mode<pin_mode::output_push_pull, 0x12>();
    EXPECT_EQ(m[ddrb_address], 0x12);

    m[pinb_address] = 0x3;
    EXPECT_EQ(port_b::read(), 0x3);

    port_b::low<0x28>();
    EXPECT_EQ(m[portb_address], 0x82);

    port_b::toggle<0x20>();
    EXPECT_EQ(m[portb_address], 0xA2);

    port_b::power_off();
    EXPECT_EQ(m[ddrb_address], 0x00);
    EXPECT_EQ(m[portb_address], 0x00);

    port_b::power_on();
    EXPECT_EQ(m[ddrb_address], 0x00);
    EXPECT_EQ(m[portb_address], 0x00);
}

TEST_F(ATmega328, pin_api_checking) {
    using namespace ::zoal::gpio;
    using port_b = mcu::port_b;
    auto pinb_address = port_b::PINx::address;
    auto ddrb_address = port_b::DDRx::address;
    auto portb_address = port_b::PORTx::address;

    uint8_t *m = zoal::test::mem<void>::avr();
    mcu::pb_00::high();
    EXPECT_EQ(m[portb_address], 0x01);

    mcu::pb_01::mode<pin_mode::output_push_pull>();
    EXPECT_EQ(m[ddrb_address], 0x02);

    EXPECT_EQ(mcu::pb_01::read(), 0x0);
    m[pinb_address] = 0x3;
    EXPECT_EQ(mcu::pb_01::read(), 0x1);

    m[portb_address] = 0x0F;
    mcu::pb_01::write(0);
    EXPECT_EQ(m[portb_address], 0x0D);
    mcu::pb_01::write(1);
    EXPECT_EQ(m[portb_address], 0x0F);

    m[portb_address] = 0x0F;
    mcu::pb_01::set<0>();
    EXPECT_EQ(m[portb_address], 0x0D);
    mcu::pb_01::set<1>();
    EXPECT_EQ(m[portb_address], 0x0F);

    m[portb_address] = 0x0F;
    mcu::pb_01::low();
    EXPECT_EQ(m[portb_address], 0x0D);
    mcu::pb_01::high();
    EXPECT_EQ(m[portb_address], 0x0F);

    m[portb_address] = 0x0F;
    mcu::pb_01::toggle();
    EXPECT_EQ(m[portb_address], 0x0D);
    mcu::pb_01::toggle();
    EXPECT_EQ(m[portb_address], 0x0F);
}

TEST_F(ATmega328, spi_address) {
    using namespace ::zoal::gpio;
    using spi = mcu::spi_00;

    auto SPCRx_address = spi::SPCRx::address;
    auto SPSRx_address = spi::SPSRx::address;
    auto SPDRx_address = spi::SPDRx::address;

    EXPECT_EQ(SPCRx_address, 0x004C);
    EXPECT_EQ(SPSRx_address, 0x004D);
    EXPECT_EQ(SPDRx_address, 0x004E);

    uint8_t *m = zoal::test::mem<void>::avr();
    spi::power_on();
    spi::enable();
    EXPECT_EQ(m[SPCRx_address], 0x40);

    spi::disable();
    spi::power_off();
    EXPECT_EQ(m[SPCRx_address], 0x00);

    auto f = []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        spi::SPSRx::ref() |= 0x80;
    };
    std::thread t(f);
    EXPECT_EQ(m[SPDRx_address], 0x00);
    spi::transfer_byte(0x31);
    EXPECT_EQ(m[SPDRx_address], 0x31);

    t.join();
}

TEST_F(ATmega328, spi_mux) {
    using namespace ::zoal::gpio;
    using port_b = mcu::port_b;
    using spi = mcu::spi_00;

    uint8_t *m = zoal::test::mem<void>::avr();
    auto ddrb_address = port_b::DDRx::address;
    auto portb_address = port_b::PORTx::address;
    EXPECT_EQ(ddrb_address, 0x0024);
    EXPECT_EQ(portb_address, 0x0025);

    mcu::mux::spi<spi, mcu::pb_03, mcu::pb_04, mcu::pb_05, mcu::pb_02>::on();
    EXPECT_EQ(m[ddrb_address], 0x2C);
    EXPECT_EQ(m[portb_address], 0x00);

    mcu::mux::spi<spi, mcu::pb_03, mcu::pb_04, mcu::pb_05, mcu::pb_02>::off();
    EXPECT_EQ(m[ddrb_address], 0x00);
    EXPECT_EQ(m[portb_address], 0x00);
}

TEST_F(ATmega328, usart_mux) {
    using namespace ::zoal::gpio;
    using usart = mcu::usart_00;

    mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::on();
    mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::off();

    //    using cas = mcu::mux::usart<usart, mcu::pd_00, mcu::pd_01, mcu::pd_04>::on_cas;
    //    zoal::test::cas_print_functor func;
    //    zoal::ct::type_list_iterator<cas>::for_each(func);
}

TEST_F(ATmega328, adc_mux) {
    using namespace ::zoal::gpio;
    using adc = mcu::adc_00;

    auto ADCSRBx_address = adc::ADCSRBx::address;
    auto ADMUXx_address = adc::ADMUXx::address;
    EXPECT_EQ(ADCSRBx_address, 0x7b);
    EXPECT_EQ(ADMUXx_address, 0x7c);

//    using cas = mcu::mux::adc<adc, mcu::pc_01>::on_cas;
//    zoal::test::cas_print_functor func;
//    zoal::ct::type_list_iterator<cas>::for_each(func);

    uint8_t *m = zoal::test::mem<void>::avr();
    EXPECT_EQ(m[ADCSRBx_address], 0x00);
    EXPECT_EQ(m[ADMUXx_address], 0x00);
    mcu::mux::adc<adc, mcu::pc_01>::on();
    EXPECT_EQ(m[ADCSRBx_address], 0x00);
    EXPECT_EQ(m[ADMUXx_address], 0x01);
    mcu::mux::adc<adc, mcu::pc_01>::off();
    EXPECT_EQ(m[ADCSRBx_address], 0x00);
    EXPECT_EQ(m[ADMUXx_address], 0x01);
}

TEST_F(ATmega328, i2c_mux) {
    using namespace ::zoal::gpio;
    using i2c = mcu::i2c_00;
    using port_c = mcu::port_c;

    auto DDRx_address = port_c::DDRx::address;
    auto PORTx_address = port_c::PORTx::address;
    EXPECT_EQ(DDRx_address, 0x27);
    EXPECT_EQ(PORTx_address, 0x28);

//    using cas = mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::on_cas;
//    zoal::test::cas_print_functor func;
//    zoal::ct::type_list_iterator<cas>::for_each(func);

    uint8_t *m = zoal::test::mem<void>::avr();

    mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::on();
    EXPECT_EQ(m[DDRx_address], 0x30);
    EXPECT_EQ(m[PORTx_address], 0x30);

    mcu::mux::i2c<i2c, mcu::pc_04, mcu::pc_05>::off();
    EXPECT_EQ(m[DDRx_address], 0x00);
    EXPECT_EQ(m[PORTx_address], 0x00);
}

TEST_F(ATmega328, timer_mux) {
    using namespace ::zoal::gpio;
    using timer = mcu::timer_00;

    uint8_t *m = zoal::test::mem<void>::avr();
    auto TCCRxA_address = timer::TCCRxA::address;
    EXPECT_EQ(TCCRxA_address, 0x44);
    EXPECT_EQ(m[TCCRxA_address], 0x00);

    mcu::mux::timer<timer, mcu::pd_06, 0>::on();
    EXPECT_EQ(m[TCCRxA_address], 0x80);

    mcu::mux::timer<timer, mcu::pd_06, 0>::off();
    EXPECT_EQ(m[TCCRxA_address], 0x00);

//    using cas = mcu::mux::timer<timer, mcu::pd_06, 0>::on_cas;
//    zoal::test::cas_print_functor func;
//    zoal::ct::type_list_iterator<cas>::for_each(func);
}

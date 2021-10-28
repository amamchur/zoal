#ifndef ZOAL_ARCH_STM32F4_I2C_HPP
#define ZOAL_ARCH_STM32F4_I2C_HPP

#include "../../../mem/reg.hpp"
#include "../../../periph/i2c_request.hpp"

namespace zoal { namespace arch { namespace stm32f4 {
    template<uintptr_t Address, class... Mixin>
    class i2c : public Mixin... {
    public:
        static constexpr uintptr_t address = Address;

        using I2Cx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_OAR1 = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_OAR2 = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_DR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_SR1 = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_SR2 = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_CCR = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_TRISE = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_FLTR = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;

        static constexpr uint32_t I2Cx_CR1_POS = 1 << 11;
        static constexpr uint32_t I2Cx_CR1_ACK = 1 << 10;
        static constexpr uint32_t I2Cx_CR1_STOP = 1 << 9;
        static constexpr uint32_t I2Cx_CR1_START = 1 << 8;
        static constexpr uint32_t I2Cx_CR1_PE = 1 << 0;

        static constexpr uint32_t I2Cx_CR2_ITBUFEN = 1 << 10; // Buffer interrupt enable
        static constexpr uint32_t I2Cx_CR2_ITEVTEN = 1 << 9; // Event interrupt enable
        static constexpr uint32_t I2Cx_CR2_ITERREN = 1 << 8; // Error interrupt enable

        static constexpr uint32_t I2Cx_SR2_BUSY = 1 << 1;

        using enable_cas = zoal::ct::type_list<typename I2Cx_CR1::template cas<0, I2Cx_CR1_PE>>;
        using disable_cas = zoal::ct::type_list<typename I2Cx_CR1::template cas<I2Cx_CR1_PE, 0>>;

        static inline void enable() {
            I2Cx_CR1::ref() |= I2Cx_CR1_PE;
        }

        static inline void disable() {
            I2Cx_CR1::ref() &= ~I2Cx_CR1_PE;
        }

        static void start(zoal::periph::i2c_request &request) {
            do {
            } while ((I2Cx_SR2 ::ref() & I2Cx_SR2_BUSY) != 0);
        }

        static void stop() {
            I2Cx_CR2::ref() &= ~(I2Cx_CR2_ITBUFEN | I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITERREN);
            I2Cx_CR1::ref() |= I2Cx_CR1_STOP;
        }

        template<class R, class C>
        static void handle_request_irq(R &request, C callback) {
            auto sr1 = I2Cx_SR1::ref();
            auto sr2 = I2Cx_SR2::ref();
            (void)sr2;


        }
    };
}}}

#endif

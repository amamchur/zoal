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

        static constexpr uint32_t I2Cx_CR2_ITBUFEN = 1 << 10;
        static constexpr uint32_t I2Cx_CR2_ITEVTEN = 1 << 9;
        static constexpr uint32_t I2Cx_CR2_ITERREN = 1 << 8;

        static constexpr uint32_t I2Cx_SR1_AF = 1 << 10; // Acknowledge failures
        static constexpr uint32_t I2Cx_SR1_ARLO = 1 << 9; // Arbitration lost (master mode)
        static constexpr uint32_t I2Cx_SR1_BERR = 1 << 8; // Bus error
        static constexpr uint32_t I2Cx_SR1_TxE = 1 << 7; // Data register empty (transmitters)
        static constexpr uint32_t I2Cx_SR1_RxNE = 1 << 6; // Data register not empty (receivers)
        static constexpr uint32_t I2Cx_SR1_BTF = 1 << 2; // Address sent (master mode)/matched (slave mode)
        static constexpr uint32_t I2Cx_SR1_ADDR = 1 << 1; // Address sent (master mode)/matched (slave mode)
        static constexpr uint32_t I2Cx_SR1_SB = 1 << 0; // Start bit (Master mode)

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

            I2Cx_SR1::ref() &= ~(I2Cx_SR1_AF | I2Cx_SR1_ARLO | I2Cx_SR1_BERR);
            I2Cx_CR1::ref() &= ~I2Cx_CR1_POS;
            I2Cx_CR1::ref() |= I2Cx_CR1_START | I2Cx_CR1_ACK;
            I2Cx_CR2::ref() |= I2Cx_CR2_ITBUFEN | I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITERREN;
        }

        static void stop() {
            I2Cx_CR2::ref() &= ~(I2Cx_CR2_ITBUFEN | I2Cx_CR2_ITEVTEN | I2Cx_CR2_ITERREN);
            I2Cx_CR1::ref() |= I2Cx_CR1_STOP;
        }

        enum i2c_events {
            //
            event_5 = I2Cx_SR1_SB,
            event_6 = I2Cx_SR1_ADDR,
            event_7 = I2Cx_SR1_RxNE,
            event_7_2 = I2Cx_SR1_RxNE | I2Cx_SR1_BTF,
            event_8 = I2Cx_SR1_TxE,
            event_8_1 = I2Cx_SR1_TxE,
            event_8_2 = I2Cx_SR1_TxE | I2Cx_SR1_BTF,
            event_9,

            event_mask = event_5 | event_6 | event_7 | event_7_2 | event_8 | event_8_1 | event_8_2 | event_9
        };

        enum i2c_errors {
            //
            none = 0,
            acknowledge_failures = I2Cx_SR1_AF,
            arbitration_lost = I2Cx_SR1_ARLO,
            bus_error = I2Cx_SR1_BERR,
            error_mask = acknowledge_failures | arbitration_lost | bus_error
        };

        template<class R, class C>
        static void handle_request_irq(R &request, C callback) {
            auto sr1 = I2Cx_SR1::ref();
            auto sr2 = I2Cx_SR2::ref();
            (void)sr2;

            auto handle_read = [](const R &request) {
                auto remaining = request.remaining_bytes();
                switch (remaining) {
                case 0:
                case 1:
                    I2Cx_CR1::ref() &= ~I2Cx_CR1_ACK;
                    I2Cx_CR1::ref() |= I2Cx_CR1_STOP;
                    break;
                default:
                    I2Cx_CR1::ref() |= I2Cx_CR1_ACK;
                    break;
                }
            };

            auto error = static_cast<i2c_errors>(sr1 & error_mask);
            if (error != none) {
                stop();
                request.complete(zoal::periph::i2c_request_status::failed);
                callback();
                return;
            }

            auto event = static_cast<i2c_events>(sr1 & event_mask);
            switch (event) {
            case event_5: {
                auto byte = request.address_rw();
                I2Cx_DR::ref() = byte;
                break;
            }
            case event_6: {
                handle_read(request);
                break;
            }
            case event_7:
            case event_7_2:
                request.push(I2Cx_DR::ref());
                if (request.eos()) {
                    request.complete(zoal::periph::i2c_request_status::finished);
                    callback();
                } else {
                    handle_read(request);
                }
                break;
            case event_8:
                if (request.eos()) {
                    stop();
                    request.complete(zoal::periph::i2c_request_status::finished);
                    callback();
                } else {
                    I2Cx_DR::ref() = request.pull();
                }
                break;
            case event_8_2:
                stop();
                request.complete(zoal::periph::i2c_request_status::finished);
                callback();
                break;
            default:
                break;
            }
        }
    };
}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_I2C_HPP
#define ZOAL_ARCH_AVR_ATMEGA_I2C_HPP

#include "../../../ct/type_list.hpp"
#include "../../../mem/cas.hpp"
#include "../../../mem/reg.hpp"
#include "../../../periph/i2c_stream.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"

#include <stdint.h> /* NOLINT */

namespace zoal { namespace arch { namespace avr { namespace atmega {
    enum : uint8_t {
        I2C_START = 0x08,
        I2C_REP_START = 0x10,
        I2C_MT_SLA_ACK = 0x18,
        I2C_MT_SLA_NACK = 0x20,
        I2C_MT_DATA_ACK = 0x28,
        I2C_MT_DATA_NACK = 0x30,
        I2C_MT_ARB_LOST = 0x38,
        I2C_MR_ARB_LOST = 0x38,
        I2C_MR_SLA_ACK = 0x40,
        I2C_MR_SLA_NACK = 0x48,
        I2C_MR_DATA_ACK = 0x50,
        I2C_MR_DATA_NACK = 0x58,
        I2C_ST_SLA_ACK = 0xA8,
        I2C_ST_ARB_LOST_SLA_ACK = 0xB0,
        I2C_ST_DATA_ACK = 0xB8,
        I2C_ST_DATA_NACK = 0xC0,
        I2C_ST_LAST_DATA = 0xC8,
        I2C_SR_SLA_ACK = 0x60,
        I2C_SR_ARB_LOST_SLA_ACK = 0x68,
        I2C_SR_GCALL_ACK = 0x70,
        I2C_SR_ARB_LOST_GCALL_ACK = 0x78,
        I2C_SR_DATA_ACK = 0x80,
        I2C_SR_DATA_NACK = 0x88,
        I2C_SR_GCALL_DATA_ACK = 0x90,
        I2C_SR_GCALL_DATA_NACK = 0x98,
        I2C_SR_STOP = 0xA0,
        I2C_NO_INFO = 0xF8,
        I2C_BUS_ERROR = 0x00,
        I2C_READ = 1,
        I2C_WRITE = 0
    };

    template<uintptr_t Address, uint8_t N>
    class i2c {
    private:
        static constexpr uint8_t TWPS1x = 1;
        static constexpr uint8_t TWPS0x = 0;

        static constexpr uint8_t TWINTx = 7;
        static constexpr uint8_t TWEAx = 6;
        static constexpr uint8_t TWSTAx = 5;
        static constexpr uint8_t TWSTOx = 4;
        static constexpr uint8_t TWWCx = 3;
        static constexpr uint8_t TWENx = 2;
        static constexpr uint8_t TWIEx = 0;

    public:
        static constexpr auto address = Address;
        static constexpr uint8_t no = N;

        using self_type = i2c<Address, N>;

        using TWBRx = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TWSRx = zoal::mem::reg<Address + 0x01, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TWARx = zoal::mem::reg<Address + 0x02, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TWDRx = zoal::mem::reg<Address + 0x03, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TWCRx = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;
        using TWAMRx = zoal::mem::reg<Address + 0x05, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;

        using enable_cas = zoal::ct::type_list<typename self_type::TWCRx::template cas<0, 1 << TWENx | 1 << TWIEx | 1 << TWEAx>>;
        using disable_cas = zoal::ct::type_list<typename self_type::TWCRx::template cas<1 << TWENx | 1 << TWIEx | 1 << TWEAx, 0>>;
        using power_on_cas = zoal::ct::type_list<zoal::mem::null_cas>;
        using power_off_cas = zoal::ct::type_list<zoal::mem::null_cas>;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            TWCRx::ref() |= 1 << TWENx | 1 << TWIEx | 1 << TWEAx;
        }

        static void disable() {
            TWCRx::ref() &= ~(1 << TWENx | 1 << TWIEx | 1 << TWEAx);
        }

        static void transmit(zoal::periph::i2c_stream<self_type> *stream) {
            stream_ = stream;
            stream_->result = zoal::periph::i2c_result::ok;
            busy_ = 1;
            TWCRx::ref() = START;
        }

        static void transmission_complete(zoal::periph::i2c_result result) {
            auto s = stream_;
            auto cb = s->callback;
            auto token = s->token;
            s->result = result;

            stream_->callback = nullptr;
            stream_ = nullptr;
            busy_ = 0;

            if (cb) {
                cb(s, token);
            }
        }

        static void wait() {
            while (busy_ || (TWCRx::ref() & 1 << TWSTOx))
                ;
        }

        static bool busy() {
            return busy_;
        }

        static constexpr uint8_t START = 1 << TWINTx | 1 << TWEAx | 1 << TWENx | 1 << TWIEx | 1 << TWSTAx;
        static constexpr uint8_t ACK = 1 << TWENx | 1 << TWIEx | 1 << TWINTx | 1 << TWEAx;
        static constexpr uint8_t NACK = 1 << TWENx | 1 << TWIEx | 1 << TWINTx;
        static constexpr uint8_t STOP = 1 << TWENx | 1 << TWIEx | 1 << TWEAx | 1 << TWINTx | 1 << TWSTOx;

        static void handle_irq() {
            auto status = static_cast<uint8_t>(TWSRx::ref() & 0xF8);
            switch (status) {
            case I2C_BUS_ERROR:
                TWCRx::ref() = STOP;
                transmission_complete(zoal::periph::i2c_result::error);
                break;
            case I2C_START:
            case I2C_REP_START:
                TWDRx::ref() = stream_->address();
                TWCRx::ref() = ACK;
                break;
            case I2C_MT_SLA_ACK:
            case I2C_MT_DATA_ACK:
                if (stream_->has_next()) {
                    TWDRx::ref() = stream_->dequeue();
                    TWCRx::ref() = ACK;
                    return;
                }

                if (stream_->stop_) {
                    TWCRx::ref() = STOP;
                    transmission_complete(zoal::periph::i2c_result::ok);
                    return;
                }

                transmission_complete(zoal::periph::i2c_result::end_of_stream);
                break;
            case I2C_MT_ARB_LOST:
                TWCRx::ref() = ACK;
                transmission_complete(zoal::periph::i2c_result::error);
                break;
            case I2C_MR_SLA_ACK:
                TWCRx::ref() = stream_->request_next() ? ACK : NACK;
                break;
            case I2C_MR_DATA_ACK:
                stream_->enqueue(TWDRx::ref());
                if (stream_->request_next()) {
                    TWCRx::ref() = ACK;
                } else {
                    TWCRx::ref() = NACK;
                }
                break;
            case I2C_MT_SLA_NACK:
                TWCRx::ref() = STOP;
                transmission_complete(zoal::periph::i2c_result::error);
                break;
            case I2C_MT_DATA_NACK:
                TWCRx::ref() = STOP;
                transmission_complete(zoal::periph::i2c_result::error);
                break;
            case I2C_MR_SLA_NACK:
                TWCRx::ref() = STOP;
                transmission_complete(zoal::periph::i2c_result::error);
                break;
            case I2C_MR_DATA_NACK:
                stream_->enqueue(TWDRx::ref());
                TWCRx::ref() = STOP;
                transmission_complete(zoal::periph::i2c_result::ok);
                break;
            default:
                break;
            }
        }

    private:
        static zoal::periph::i2c_stream<self_type> *stream_;
        static volatile uint8_t busy_;
    };

    template<uintptr_t Address, uint8_t N>
    volatile uint8_t i2c<Address, N>::busy_ = 0;

    template<uintptr_t Address, uint8_t N>
    zoal::periph::i2c_stream<i2c<Address, N>> *i2c<Address, N>::stream_ = nullptr;
}}}}

#endif

#ifndef ZOAL_ARCH_AVR_ATMEGA_I2C_HPP
#define ZOAL_ARCH_AVR_ATMEGA_I2C_HPP

#include "../../../mem/segment.hpp"
#include "../../../periph/i2c_config.hpp"
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

    class i2c_stream {
    public:
        explicit i2c_stream(void *buffer)
            : index(0)
            , length(0)
            , slawr(0) {
            data = (uint8_t *)buffer;
        }

        i2c_stream &write(uint8_t address) {
            slawr = address << 1;
            index = 0;
            length = 0;
            return *this;
        }

        i2c_stream &read(uint8_t address, uint16_t count) {
            slawr = static_cast<uint8_t>(address << 1 | 1);
            index = 0;
            length = count;
            return *this;
        }

        uint8_t address() const {
            return slawr;
        }

        uint16_t size() const {
            return length;
        }

        bool next() const {
            return index < length;
        }

        uint8_t dequeue() {
            return data[index++];
        }

        i2c_stream &value(uint8_t value) {
            data[length++] = value;
            return *this;
        }

        i2c_stream &enqueue(uint8_t value) {
            data[index++] = value;
            return *this;
        }

        uint16_t index;
        uint16_t length;
        uint8_t slawr;
        uint8_t *data;
    };

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
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

        using self_type = i2c<Address, BufferSize, N>;

        static constexpr uintptr_t TWBRx = 0;
        static constexpr uintptr_t TWSRx = 1;
        static constexpr uintptr_t TWARx = 2;
        static constexpr uintptr_t TWDRx = 3;
        static constexpr uintptr_t TWCRx = 4;
        static constexpr uintptr_t TWAMRx = 5;

        static uint8_t buffer[BufferSize];
        static i2c_stream stream_;
        static volatile uint8_t busy;
        static volatile uint8_t error;

        static void (*callback)(void *);

        static void *token;

        static void power_on() {}

        static void power_off() {}

        static void enable() {
            mem[TWCRx] = 1 << TWENx | 1 << TWIEx | 1 << TWEAx;
        }

        static void disable() {
            mem[TWCRx] &= ~(1 << TWENx | 1 << TWIEx | 1 << TWEAx);
        }

        template<class Config>
        static void begin() {
            //            I2CSDA::high();
            //            I2CSCL::high();

            mem[TWSRx] &= ~(1 << TWPS0x | 1 << TWPS1x);
            mem[TWBRx] = ((Config::freq / Config::i2c_freq) - 16) / 2;
        }

        static i2c_stream &stream(void *extBuffer = nullptr) {
            stream_.index = 0;
            stream_.data = extBuffer == nullptr ? (uint8_t *)buffer : (uint8_t *)extBuffer;
            return stream_;
        }

        static void transmit(void (*cb)(void *) = nullptr, void *t = nullptr) {
            callback = cb;
            token = t;

            busy = 1;
            mem[TWCRx] = START;

            if (cb == nullptr) {
                wait();
            }
        }

        static void transmission_complete(uint8_t code) {
            void (*cb)(void *) = callback;
            callback = nullptr;
            busy = 0;
            error = code;
            if (cb) {
                cb(token);
            }
        }

        static void wait() {
            while (busy || (mem[TWCRx] & 1 << TWSTOx))
                ;
        }

        static constexpr uint8_t START = 1 << TWINTx | 1 << TWEAx | 1 << TWENx | 1 << TWIEx | 1 << TWSTAx;
        static constexpr uint8_t ACK = 1 << TWENx | 1 << TWIEx | 1 << TWINTx | 1 << TWEAx;
        static constexpr uint8_t NACK = 1 << TWENx | 1 << TWIEx | 1 << TWINTx;
        static constexpr uint8_t STOP = 1 << TWENx | 1 << TWIEx | 1 << TWEAx | 1 << TWINTx | 1 << TWSTOx;

        static void handle_irq() {
            auto status = static_cast<uint8_t>(mem[TWSRx] & 0xF8);
            switch (status) {
            case I2C_BUS_ERROR:
                mem[TWCRx] = STOP;
                transmission_complete(1);
                break;
            case I2C_START:
            case I2C_REP_START:
                mem[TWDRx] = stream_.address();
                mem[TWCRx] = ACK;
                break;
            case I2C_MT_SLA_ACK:
            case I2C_MT_DATA_ACK:
                if (stream_.next()) {
                    mem[TWDRx] = stream_.dequeue();
                    mem[TWCRx] = ACK;
                } else {
                    mem[TWCRx] = STOP;
                    transmission_complete(0);
                }
                break;
            case I2C_MT_ARB_LOST:
                mem[TWCRx] = ACK;
                transmission_complete(2);
                break;
            case I2C_MR_SLA_ACK:
                mem[TWCRx] = stream_.next() ? ACK : NACK;
                break;
            case I2C_MR_DATA_ACK:
                stream_.enqueue(mem[TWDRx]);
                if (stream_.next()) {
                    mem[TWCRx] = ACK;
                } else {
                    mem[TWCRx] = NACK;
                }
                break;
            case I2C_MT_SLA_NACK:
            case I2C_MT_DATA_NACK:
            case I2C_MR_SLA_NACK:
            case I2C_MR_DATA_NACK:
                mem[TWCRx] = STOP;
                transmission_complete(0);
                break;
            default:
                break;
            }
        }

    private:
        static zoal::mem::segment<uint8_t, Address> mem;
    };

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
    volatile uint8_t i2c<Address, N, BufferSize>::busy = 0;

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
    volatile uint8_t i2c<Address, N, BufferSize>::error = 0;

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
    uint8_t i2c<Address, N, BufferSize>::buffer[BufferSize];

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
    i2c_stream i2c<Address, N, BufferSize>::stream_(i2c<Address, N, BufferSize>::buffer);

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
    void (*i2c<Address, N, BufferSize>::callback)(void *) = nullptr;

    template<uintptr_t Address, uint8_t N, uint8_t BufferSize>
    void *i2c<Address, N, BufferSize>::token = nullptr;
}}}}

#endif

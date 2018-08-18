#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_I2C_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_I2C_HPP

#include <stdint.h> /* NOLINT */
#include "../../../data/ring_buffer.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/yield.hpp"
#include "../../../utils/nop.hpp"
#include "../../../utils/memory_segment.hpp"
#include "../../../periph/i2c_config.hpp"

namespace zoal { namespace arch { namespace avr {
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
        i2c_stream(void *buffer)
                : index(0), length(0), slawr(0) {
            data = (uint8_t *) buffer;
        }

        i2c_stream &write(uint8_t address) {
            slawr = address << 1;
            index = 0;
            length = 0;
            return *this;
        }

        i2c_stream &read(uint8_t address, uint16_t count) {
            slawr = address << 1 | 1;
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

    template<uintptr_t BaseAddress, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    class i2c {
    private:
        static void waitI2C() {
            zoal::utils::interrupts enableInterrupts(true);
            zoal::utils::nop<1>::place();
        }

    public:
        typedef zoal::utils::yield<> yield;

        template<uintptr_t Size>
        using Buffer = typename ::zoal::data::ring_buffer<uint8_t, Size, yield::place>;

        typedef i2c<BaseAddress, BufferSize, I2CSDA, I2CSCL> Class;

        static constexpr uintptr_t TWBRx = 0;
        static constexpr uintptr_t TWSRx = 1;
        static constexpr uintptr_t TWARx = 2;
        static constexpr uintptr_t TWDRx = 3;
        static constexpr uintptr_t TWCRx = 4;
        static constexpr uintptr_t TWAMRx = 5;

        static constexpr uint8_t TWAM6x = 7;
        static constexpr uint8_t TWAM5x = 6;
        static constexpr uint8_t TWAM4x = 5;
        static constexpr uint8_t TWAM3x = 4;
        static constexpr uint8_t TWAM2x = 3;
        static constexpr uint8_t TWAM1x = 2;
        static constexpr uint8_t TWAM0x = 1;

        static constexpr uint8_t TWS7x = 7;
        static constexpr uint8_t TWS6x = 6;
        static constexpr uint8_t TWS5x = 5;
        static constexpr uint8_t TWS4x = 4;
        static constexpr uint8_t TWS3x = 3;
        static constexpr uint8_t TWPS1x = 1;
        static constexpr uint8_t TWPS0x = 0;

        static constexpr uint8_t TWINTx = 7;
        static constexpr uint8_t TWEAx = 6;
        static constexpr uint8_t TWSTAx = 5;
        static constexpr uint8_t TWSTOx = 4;
        static constexpr uint8_t TWWCx = 3;
        static constexpr uint8_t TWENx = 2;
        static constexpr uint8_t TWIEx = 0;


        static uint8_t buffer[BufferSize];
        static i2c_stream stream_;
        static volatile uint8_t busy;
        static volatile uint8_t error;

        static void (*callback)(void *);

        static void *token;

        template<class Config>
        static void begin() {
            I2CSDA::high();
            I2CSCL::high();

            mem[TWSRx] &= ~(1 << TWPS0x | 1 << TWPS1x);
            mem[TWBRx] = ((Config::Freq / Config::I2CFreq) - 16) / 2;
            mem[TWCRx] = 1 << TWENx | 1 << TWIEx | 1 << TWEAx;
        }

        static void end() {
            mem[TWCRx] &= ~(1 << TWENx | 1 << TWIEx | 1 << TWEAx);

            I2CSDA::low();
            I2CSCL::low();
        }

        static i2c_stream &stream(void *extBuffer = nullptr) {
            stream_.index = 0;
            stream_.data = extBuffer == nullptr ? (uint8_t *) buffer : (uint8_t *) extBuffer;
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
            while (busy || (mem[TWCRx] & 1 << TWSTOx));
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
        static zoal::utils::memory_segment<uint8_t, BaseAddress> mem;
    };

    template<uintptr_t Address, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    volatile uint8_t i2c<Address, BufferSize, I2CSDA, I2CSCL>::busy = 0;

    template<uintptr_t Address, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    volatile uint8_t i2c<Address, BufferSize, I2CSDA, I2CSCL>::error = 0;

    template<uintptr_t Address, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    uint8_t i2c<Address, BufferSize, I2CSDA, I2CSCL>::buffer[BufferSize];

    template<uintptr_t Address, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    ::zoal::arch::avr::i2c_stream i2c<Address, BufferSize, I2CSDA, I2CSCL>::stream_(
            ::zoal::arch::avr::i2c<Address, BufferSize, I2CSDA, I2CSCL>::buffer);

    template<uintptr_t Address, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    void (*i2c<Address, BufferSize, I2CSDA, I2CSCL>::callback)(void *) = nullptr;

    template<uintptr_t Address, uint8_t BufferSize, class I2CSDA, class I2CSCL>
    void *i2c<Address, BufferSize, I2CSDA, I2CSCL>::token = nullptr;
}}}

#endif

#ifndef ZOAL_ARCH_STM32F3_I2C_HPP
#define ZOAL_ARCH_STM32F3_I2C_HPP

#include "../../../mem/reg.hpp"
#include "../../../periph/i2c_request.hpp"

namespace zoal { namespace arch { namespace stm32f3 {
    template<uintptr_t Address, class... Mixin>
    class i2c : public Mixin... {
    public:
        static constexpr uintptr_t address = Address;

        using I2Cx_CR1 = zoal::mem::reg<Address + 0x00, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_CR2 = zoal::mem::reg<Address + 0x04, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_OAR1 = zoal::mem::reg<Address + 0x08, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_OAR2 = zoal::mem::reg<Address + 0x0C, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_TIMINGR = zoal::mem::reg<Address + 0x10, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_TIMEOUTR = zoal::mem::reg<Address + 0x14, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_ISR = zoal::mem::reg<Address + 0x18, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_ICR = zoal::mem::reg<Address + 0x1C, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_PECR = zoal::mem::reg<Address + 0x20, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_RXDR = zoal::mem::reg<Address + 0x24, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;
        using I2Cx_TXDR = zoal::mem::reg<Address + 0x28, zoal::mem::reg_io::read_write, uint32_t, 0x0000FFFF>;

        static constexpr uint32_t I2Cx_CR1_ERRIE = 1 << 7; // Error interrupts enable
        static constexpr uint32_t I2Cx_CR1_TCIE = 1 << 6; // STOP detection Interrupt enable
        static constexpr uint32_t I2Cx_CR1_STOPIE = 1 << 5; // STOP detection Interrupt enable
        static constexpr uint32_t I2Cx_CR1_NACKIE = 1 << 4; // Not acknowledge received Interrupt enable
        static constexpr uint32_t I2Cx_CR1_RXIE = 1 << 2; //  RX Interrupt enable
        static constexpr uint32_t I2Cx_CR1_TXIE = 1 << 1; // TX Interrupt enable
        static constexpr uint32_t I2Cx_CR1_PE = 1 << 0; // Peripheral enable

        static constexpr uint32_t I2Cx_CR2_STOP = 1 << 14; // Stop generation
        static constexpr uint32_t I2Cx_CR2_START = 1 << 13; // Start generation
        static constexpr uint32_t I2Cx_CR2_RD_WRN = 1 << 10; // Transfer direction (master mode)

        static constexpr uint32_t I2Cx_ISR_BUSY = 1 << 15; // Bus busy
        static constexpr uint32_t I2Cx_ISR_ARLO = 1 << 9; // Arbitration lost
        static constexpr uint32_t I2Cx_ISR_BERR = 1 << 8; // Bus error
        static constexpr uint32_t I2Cx_ISR_TC = 1 << 6; // Transfer Complete (master mode)
        static constexpr uint32_t I2Cx_ISR_STOPF = 1 << 5; // Transfer Complete (master mode)
        static constexpr uint32_t I2Cx_ISR_NACKF = 1 << 4; // Not Acknowledge received flag
        static constexpr uint32_t I2Cx_ISR_RXNE = 1 << 2; // Receive data register not empty (receivers)
        static constexpr uint32_t I2Cx_ISR_TXIS = 1 << 1; // Transmit interrupt status (transmitters)
        static constexpr uint32_t I2Cx_ISR_TXE = 1 << 0; // Transmit data register empty (transmitters)

        static inline void enable() {
            I2Cx_CR1::ref() |= I2Cx_CR1_PE;
        }

        static inline void disable() {
            I2Cx_CR1::ref() &= ~I2Cx_CR1_PE;
        }

        static void start(zoal::periph::i2c_request &request) {
            do {
            } while ((I2Cx_ISR::ref() & I2Cx_ISR_BUSY) != 0);

            I2Cx_ICR::ref() |= static_cast<uint32_t>(event_mask) | static_cast<uint32_t>(error_mask);

            configure_control_registers(request);

            I2Cx_CR2::ref() |= I2Cx_CR2_START;
        }

        static void stop() {
            I2Cx_CR1::ref() &= ~(I2Cx_CR1_TXIE | I2Cx_CR1_RXIE | I2Cx_CR1_NACKIE | I2Cx_CR1_STOPIE | I2Cx_CR1_TCIE | I2Cx_CR1_ERRIE);
            I2Cx_ICR::ref() |= static_cast<uint32_t>(error_mask) | I2Cx_ISR_STOPF;
        }

        enum i2c_events {
            //
            tx_empty = I2Cx_ISR_TXIS | I2Cx_ISR_TXE,
            rx_not_empty = I2Cx_ISR_RXNE,

            transfer_complete_tx = I2Cx_ISR_TC | I2Cx_ISR_TXE,
            transfer_complete_rx = I2Cx_ISR_TC | I2Cx_ISR_RXNE,

            stop_event_1 = I2Cx_ISR_STOPF | I2Cx_ISR_TXE,
            stop_event_2 = I2Cx_ISR_STOPF | I2Cx_ISR_RXNE,
            stop_event_3 = I2Cx_ISR_STOPF,

            rx_event_mask = I2Cx_ISR_RXNE | I2Cx_ISR_TC | I2Cx_ISR_STOPF,
            tx_event_mask = I2Cx_ISR_TXE | I2Cx_ISR_TC | I2Cx_ISR_STOPF | I2Cx_ISR_TXIS,
            event_mask = tx_empty | rx_not_empty | transfer_complete_tx | transfer_complete_rx | stop_event_1 | stop_event_2
        };

        enum i2c_errors {
            //
            none = 0,
            acknowledge_failures = I2Cx_ISR_NACKF,
            arbitration_lost = I2Cx_ISR_ARLO,
            bus_error = I2Cx_ISR_BERR,
            error_mask = acknowledge_failures | arbitration_lost | bus_error
        };

        template<class R, class C>
        static void handle_request_irq(R &request, C callback) {
            auto isr = I2Cx_ISR::ref();

            auto error = static_cast<i2c_errors>(isr & error_mask);
            if (error != none) {
                stop();
                request.complete(zoal::periph::i2c_request_status::failed);
                callback();
                return;
            }

            auto cr2 = I2Cx_CR2::ref();
            auto rx = (cr2 & I2Cx_CR2_RD_WRN) == I2Cx_CR2_RD_WRN;
            auto mask = rx ? rx_event_mask : tx_event_mask;
            auto event = static_cast<i2c_events>(isr & mask);
            switch (event) {
            case tx_empty: {
                I2Cx_TXDR::ref() = request.pull();
                break;
            }
            case rx_not_empty: {
                request.push(I2Cx_RXDR::ref());
                break;
            }
            case transfer_complete_tx:
            case transfer_complete_rx: {
                if (request.eos()) {
                    I2Cx_CR2::ref() |= I2Cx_CR2_STOP;
                } else {
                    configure_control_registers(request);
                    I2Cx_CR2::ref() |= I2Cx_CR2_START;
                }
                break;
            }
            case stop_event_1:
            case stop_event_2:
            case stop_event_3:
                stop();
                request.complete(zoal::periph::i2c_request_status::finished);
                callback();
                break;
            default:
                break;
            }
        }

    private:
        static void configure_control_registers(zoal::periph::i2c_request &request) {
            uint32_t dev_add = request.address_rw() & 0xFE;
            uint32_t read_bit = (request.address_rw() & 0x01) << 10;
            uint32_t count = request.remaining_bytes();
            uint32_t nbytes = ((count >= 255) ? 0xFF : count) << 16; // Bits 23:16 NBYTES[7:0]: Number of bytes
            uint32_t reload = (count > 255 ? 1 : 0) << 24;
            uint32_t auto_end = (reload ? 0 : 1) << 25;
            uint32_t cr2 = dev_add | read_bit | nbytes | reload | auto_end;

            I2Cx_CR1::ref() |= I2Cx_CR1_TXIE | I2Cx_CR1_RXIE | I2Cx_CR1_NACKIE | I2Cx_CR1_STOPIE | I2Cx_CR1_TCIE | I2Cx_CR1_ERRIE;
            I2Cx_CR2::ref() = cr2;
        }
    };
}}}

#endif

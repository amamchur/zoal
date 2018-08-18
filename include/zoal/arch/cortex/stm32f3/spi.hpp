#pragma once

#ifndef ZOAL_GPIO_STM32F3_HARDWARE_SPI_HPP
#define ZOAL_GPIO_STM32F3_HARDWARE_SPI_HPP

#include <stdint.h>
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace gpio { namespace stm32f3 {
    enum SPI_FLAGS : uint16_t {
        CR1_CPHA = 0x0001,          /*!< Clock Phase */
        CR1_CPOL = 0x0002,          /*!< Clock Polarity */
        CR1_MSTR = 0x0004,          /*!< Master Selection */
        CR1_BR = 0x0038,            /*!< BR[2:0] bits (Baud Rate Control) */
        CR1_BR_0 = 0x0008,          /*!< Bit 0 */
        CR1_BR_1 = 0x0010,          /*!< Bit 1 */
        CR1_BR_2 = 0x0020,          /*!< Bit 2 */
        CR1_SPE = 0x0040,           /*!< SPI Enable */
        CR1_LSBFIRST = 0x0080,      /*!< Frame Format */
        CR1_SSI = 0x0100,           /*!< Internal slave select */
        CR1_SSM = 0x0200,           /*!< Software slave management */
        CR1_RXONLY = 0x0400,        /*!< Receive only */
        CR1_CRCL = 0x0800,          /*!< CRC Length */
        CR1_CRCNEXT = 0x1000,       /*!< Transmit CRC next */
        CR1_CRCEN = 0x2000,         /*!< Hardware CRC calculation enable */
        CR1_BIDIOE = 0x4000,        /*!< Output enable in bidirectional mode */
        CR1_BIDIMODE = 0x8000,      /*!< Bidirectional data mode enable */

        CR2_RXDMAEN = 0x0001,       /*!< Rx Buffer DMA Enable */
        CR2_TXDMAEN = 0x0002,       /*!< Tx Buffer DMA Enable */
        CR2_SSOE = 0x0004,          /*!< SS Output Enable */
        CR2_NSSP = 0x0008,          /*!< NSS pulse management Enable */
        CR2_FRF = 0x0010,           /*!< Frame Format Enable */
        CR2_ERRIE = 0x0020,         /*!< Error Interrupt Enable */
        CR2_RXNEIE = 0x0040,        /*!< RX buffer Not Empty Interrupt Enable */
        CR2_TXEIE = 0x0080,         /*!< Tx buffer Empty Interrupt Enable */
        CR2_DS = 0x0F00,            /*!< DS[3:0] Data Size */
        CR2_DS_0 = 0x0100,          /*!< Bit 0 */
        CR2_DS_1 = 0x0200,          /*!< Bit 1 */
        CR2_DS_2 = 0x0400,          /*!< Bit 2 */
        CR2_DS_3 = 0x0800,          /*!< Bit 3 */
        CR2_FRXTH = 0x1000,         /*!< FIFO reception Threshold */
        CR2_LDMARX = 0x2000,        /*!< Last DMA transfer for reception */
        CR2_LDMATX = 0x4000,        /*!< Last DMA transfer for transmission */
    };

    template<uintptr_t BaseAddr, class RCController, uint32_t RCCMask>
    class spi_controller {
    public:
        using Class = spi_controller<BaseAddr, RCController, RCCMask>;

        spi_controller() = delete;

        static constexpr uintptr_t SPIx_CR1 = 0x00;
        static constexpr uintptr_t SPIx_CR2 = 0x04;
        static constexpr uintptr_t SPIx_SR = 0x08;
        static constexpr uintptr_t SPIx_DR = 0x0C;
        static constexpr uintptr_t SPIx_CRCPR = 0x10;
        static constexpr uintptr_t SPIx_RXCRCR = 0x14;
        static constexpr uintptr_t SPIx_TXCRCR = 0x18;
        static constexpr uintptr_t SPIx_I2SCFGR = 0x1C;
        static constexpr uintptr_t SPIx_I2SPR = 0x20;

        static inline void transfer_byte(uint8_t data) {
            // SPI_I2S_FLAG_TXE | SPI_I2S_FLAG_RXNE
            const uint16_t TxRxBusyMask = 0x0083;
            const uint16_t TxEmptyAndRxNotEmptyAndNotBusy = 0x0003;

            mem[SPIx_DR] = data;

            while ((mem[SPIx_SR] & TxRxBusyMask) != TxEmptyAndRxNotEmptyAndNotBusy);
            return mem[SPIx_DR];
        }

        static inline void enable() {
            RCController::instance()->APB2ENR |= RCCMask;
            mem[SPIx_CR1] |= CR1_SPE;
        }

        static inline void disable() {
            RCController::instance()->APB2ENR &= ~RCCMask;
            mem[SPIx_CR1] &= ~CR1_SPE;
        }

    private:
        static zoal::utils::memory_segment<uint32_t, BaseAddr> mem;
    };

    template<class Ctrl, uint8_t MSBF = 1, uint8_t Mode = 0>
    class concrete_spi {
    public:
        using SPIController = Ctrl;

        void inline begin() {
            SPIController::enable();
        }

        void inline setup() {
            auto ctrl = SPIController::instance();
            auto CR1 = ctrl->CR1 & 0x3040;
            CR1 |= CR1_MSTR | CR1_SSI; // Master mode
            CR1 |= Mode;
            CR1 |= MSBF ? 0 : CR1_LSBFIRST;

            auto CR2 = ctrl->CR2 & CR2_DS;
            CR2 |= 0x0700; // Configure SPI data Size to 8 bit

            ctrl->CR1 = CR1;
            ctrl->CR2 = CR2;
        }

        template<class T>
        void inline transfer(T data) {
            auto ctrl = SPIController::instance();
            uint8_t i = MSBF ? sizeof(T) * 8 : 0;
            uint8_t j = MSBF ? 0 : sizeof(T) * 8;
            do {
                i += MSBF ? -8 : 8;
                ctrl->transferByte((data >> i) & 0xFF);
            } while (i != j);
        }
    };

    template<class Ctrl>
    class spi {
    public:
        using MSBF_MODE_0 = concrete_spi<Ctrl, 1, 0>;
        using MSBF_MODE_1 = concrete_spi<Ctrl, 1, 1>;
        using MSBF_MODE_2 = concrete_spi<Ctrl, 1, 2>;
        using MSBF_MODE_3 = concrete_spi<Ctrl, 1, 3>;

        using LSBF_MODE_0 = concrete_spi<Ctrl, 0, 0>;
        using LSBF_MODE_1 = concrete_spi<Ctrl, 0, 1>;
        using LSBF_MODE_2 = concrete_spi<Ctrl, 0, 2>;
        using LSBF_MODE_3 = concrete_spi<Ctrl, 0, 3>;
    };
}}}

#endif

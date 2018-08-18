#pragma once

#ifndef ZOAL_ARCH_ATMEL_AVR_HARDWARE_USART_HPP
#define ZOAL_ARCH_ATMEL_AVR_HARDWARE_USART_HPP

#include "../../../data/ring_buffer.hpp"
#include "../../../periph/usart_config.hpp"
#include "../../../utils/interrupts.hpp"
#include "../../../utils/nop.hpp"
#include "../../../gpio/pin_mode.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    void waitUSART() {
        zoal::utils::interrupts(true);
        zoal::utils::nop<1>::place();
    }

    template<uintptr_t BaseAddr, uintptr_t TxSize, uintptr_t RxSize, class Clock, class IrqCtrl, class TX, class RX>
    class usart : public Clock, public IrqCtrl {
    public:
        typedef usart<BaseAddr, TxSize, RxSize, Clock, IrqCtrl, TX, RX> Class;

        template<uintptr_t Size>
        using Buffer = typename ::zoal::data::ring_buffer<uint8_t, Size, waitUSART>;

        static Buffer<TxSize> tx;
        static Buffer<RxSize> rx;

        volatile uint16_t SR;
        volatile uint16_t RESERVED0;
        volatile uint16_t DR;
        volatile uint16_t RESERVED1;
        volatile uint16_t BRR;
        volatile uint16_t RESERVED2;
        volatile uint16_t CR1;
        volatile uint16_t RESERVED3;
        volatile uint16_t CR2;
        volatile uint16_t RESERVED4;
        volatile uint16_t CR3;
        volatile uint16_t RESERVED5;
        volatile uint16_t GTPR;
        volatile uint16_t RESERVED6;

        static const uint16_t SR_RXNE = 0x0020;
        static const uint16_t SR_TXE = 0x0080;
        static const uint16_t CR1_RE = 0x0004;
        static const uint16_t CR1_TE = 0x0008;
        static const uint16_t CR1_RXNEIE = 0x0020;
        static const uint16_t CR1_TXEIE = 0x0080;
        static const uint16_t CR1_UE = 0x2000;

        constexpr static inline Class *instance() {
            return (Class *) BaseAddr;
        }

        template<class Config>
        static void begin() {
            using namespace zoal::gpio;

            Clock::enable();
            IrqCtrl::enableIrq();

            TX::Controller::enable();
            TX::Controller::instance()->alternatePushPullMode(TX::Mask);

            RX::Controller::enable();
            RX::template mode<pin_mode::input_floating>();

            double usartDiv = Config::Freq / 16.0 / Config::BaudRate;
            uint16_t usartInt = usartDiv;
            usartDiv -= usartInt;
            usartDiv *= 16;

            auto usart = instance();
            usart->BRR = (usartInt << 4) + (uint16_t) usartDiv;
            usart->CR1 = CR1_RXNEIE | CR1_UE | CR1_RE | CR1_TE;
        }

        static void write(uint8_t data) {
            zoal::utils::interrupts ni(false);
            tx.enqueue(data, true);
            instance()->CR1 |= CR1_TXEIE;
        }

        template<class F>
        static void write(F &fn) {
            uint8_t data = 0;
            while (fn(data)) {
                write(data);
            }
        }

        template<class F>
        static void read(F &fn) {
            while (fn(rx.dequeue(true)));
        }

        static void handleIrq() {
            auto usart = instance();
            if (usart->SR & SR_TXE) {
                if (tx.empty()) {
                    usart->CR1 &= ~CR1_TXEIE;
                } else {
                    usart->DR = tx.dequeue();
                }
            }

            if (usart->SR & SR_RXNE) {
                usart->SR &= ~SR_RXNE;
                rx.enqueue((uint8_t) usart->DR);
            }
        }
    };

    template<uintptr_t BaseAddr, uintptr_t TxSize, uintptr_t RxSize, class Clock, class IrqCtrl, class TX, class RX>
    ::zoal::data::ring_buffer<uint8_t, TxSize, waitUSART> usart<BaseAddr, TxSize, RxSize, Clock, IrqCtrl, TX, RX>::tx;

    template<uintptr_t BaseAddr, uintptr_t TxSize, uintptr_t RxSize, class Clock, class IrqCtrl, class TX, class RX>
    ::zoal::data::ring_buffer<uint8_t, RxSize, waitUSART> usart<BaseAddr, TxSize, RxSize, Clock, IrqCtrl, TX, RX>::rx;
}}}

#endif

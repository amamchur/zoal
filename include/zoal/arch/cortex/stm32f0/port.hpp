#ifndef ZOAL_GPIO_STM32F0_PARALLEL_IO_CONTROLLER_HPP
#define ZOAL_GPIO_STM32F0_PARALLEL_IO_CONTROLLER_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace gpio { namespace stm32f0 {
    template<uintptr_t Address, class Clock>
    class port :public Clock {
    public:
        using register_type = uint32_t;

        static constexpr uintptr_t address = Address;

        static constexpr uintptr_t GPIOx_MODER = 0x00;
        static constexpr uintptr_t GPIOx_OTYPER = 0x04;
        static constexpr uintptr_t GPIOx_OSPEEDR = 0x08;
        static constexpr uintptr_t GPIOx_PUPDR = 0x0C;
        static constexpr uintptr_t GPIOx_IDR = 0x10;
        static constexpr uintptr_t GPIOx_ODR = 0x14;
        static constexpr uintptr_t GPIOx_BSRR = 0x18;
        static constexpr uintptr_t GPIOx_LCKR = 0x1C;
        static constexpr uintptr_t GPIOx_AFRL = 0x20;
        static constexpr uintptr_t GPIOx_AFRH = 0x24;
        static constexpr uintptr_t GPIOx_BRR = 0x28;

        port() = delete;

        static inline register_type read() {
            return mem[GPIOx_IDR];
        }

        static inline void low(register_type mask = ~(register_type) 0) {
            mem[GPIOx_BRR] = mask;
        }

        static inline void high(register_type mask = ~(register_type) 0) {
            mem[GPIOx_BSRR] = mask;
        }

        static inline void toggle(register_type mask = ~(register_type) 0) {
            auto data = mem[GPIOx_ODR];
            mem[GPIOx_BRR] = data & mask;
            mem[GPIOx_BSRR] = ~data & mask;
        }

        template<::zoal::gpio::pin_mode PinMode>
        static inline void mode(register_type mask) {
            using namespace ::zoal::gpio;

            uint32_t pin_direction = 0;
            uint32_t output_type = 0;
            uint16_t pull_up_pull_down = 0;
            const uint32_t speedSettings = 0x1; // 10 MHz
            switch (PinMode) {
                case pin_mode::input_floating:
                    output_type = mask;
                    break;
                case pin_mode::input_pull_down:
                    pull_up_pull_down = 0x02;
                    break;
                case pin_mode::input_pull_up:
                    pull_up_pull_down = 0x01;
                    break;
                case pin_mode::output_open_drain:
                    output_type = mask;
                    pin_direction = 0x1;
                    break;
                case pin_mode::output_push_pull:
                default:
                    pin_direction = 0x1;
                    break;
            }

            register_type vOSPEEDR = mem[GPIOx_OSPEEDR];
            register_type vOTYPER = (mem[GPIOx_OTYPER] & ~mask) | output_type;
            register_type vMODER = mem[GPIOx_MODER];
            register_type vPUPDR = mem[GPIOx_PUPDR];

            for (register_type i = 0; i < 16; i++) {
                auto v = static_cast<uint8_t>((mask >> i) & 0x1);
                if (!v) {
                    continue;
                }

                register_type offset = i << 1;
                auto unmask = ~static_cast<register_type>(0x3 << offset);

                vOSPEEDR &= unmask;
                vMODER &= unmask;
                vPUPDR &= unmask;

                vOSPEEDR |= speedSettings << offset;
                vMODER |= pin_direction << offset;
                vPUPDR |= pull_up_pull_down << offset;
            }

            switch (PinMode) {
                case pin_mode::output_open_drain:
                case pin_mode::output_push_pull:
                    mem[GPIOx_OSPEEDR] = vOSPEEDR;
                    mem[GPIOx_OTYPER] = vOTYPER;
                    break;
                default:
                    break;
            }
            mem[GPIOx_MODER] = vMODER;
            mem[GPIOx_PUPDR] = vPUPDR;
        }

        template<::zoal::gpio::pin_mode PinMode, register_type mask>
        static inline void mode() {
            using namespace ::zoal::gpio;

            uint32_t pin_direction = 0;
            uint32_t output_type = 0;
            uint16_t pull_up_pull_down = 0;
            const uint32_t speedSettings = 0x1; // 10 MHz
            switch (PinMode) {
                case pin_mode::input_floating:
                    output_type = mask;
                    break;
                case pin_mode::input_pull_down:
                    pull_up_pull_down = 0x02;
                    break;
                case pin_mode::input_pull_up:
                    pull_up_pull_down = 0x01;
                    break;
                case pin_mode::output_open_drain:
                    output_type = mask;
                    pin_direction = 0x1;
                    break;
                case pin_mode::output_push_pull:
                default:
                    pin_direction = 0x1;
                    break;
            }

            register_type vOSPEEDR = mem[GPIOx_OSPEEDR];
            register_type vOTYPER = (mem[GPIOx_OTYPER] & ~mask) | output_type;
            register_type vMODER = mem[GPIOx_MODER];
            register_type vPUPDR = mem[GPIOx_PUPDR];

            for (register_type i = 0; i < 16; i++) {
                auto v = static_cast<uint8_t>((mask >> i) & 0x1);
                if (!v) {
                    continue;
                }

                register_type offset = i << 1;
                auto unmask = ~static_cast<register_type>(0x3 << offset);

                vOSPEEDR &= unmask;
                vMODER &= unmask;
                vPUPDR &= unmask;

                vOSPEEDR |= speedSettings << offset;
                vMODER |= pin_direction << offset;
                vPUPDR |= pull_up_pull_down << offset;
            }

            switch (PinMode) {
                case pin_mode::output_open_drain:
                case pin_mode::output_push_pull:
                    mem[GPIOx_OSPEEDR] = vOSPEEDR;
                    mem[GPIOx_OTYPER] = vOTYPER;
                    break;
                default:
                    break;
            }
            mem[GPIOx_MODER] = vMODER;
            mem[GPIOx_PUPDR] = vPUPDR;
        }
    private:
        static zoal::utils::memory_segment<uint32_t, Address> mem;
    };

    template<uintptr_t Address, class Clock>
    zoal::utils::memory_segment<uint32_t, Address> port<Address, Clock>::mem;
}}}

#endif

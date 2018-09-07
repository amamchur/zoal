#pragma once

#ifndef ZOAL_ARCH_STM32F1_PORT_HPP
#define ZOAL_ARCH_STM32F1_PORT_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template <::zoal::gpio::pin_mode PinMode>
    struct pin_mode_to_cnf_mode {
    };

    template <>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::input_floating> {
        static constexpr uint32_t value = 0x4 | 0x0;
    };

    template <>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::input_pull_down> {
        static constexpr uint32_t value = 0x8 | 0x0;
    };

    template <>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::input_pull_up> {
        static constexpr uint32_t value = 0x8 | 0x0;
    };

    template <>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::output_open_drain> {
        static constexpr uint32_t value = 0x4 | 0x1;
    };

    template <>
    struct pin_mode_to_cnf_mode<::zoal::gpio::pin_mode::output_push_pull> {
        static constexpr uint32_t value = 0x0 | 0x1;
    };

    template<uintptr_t Address, class Clock>
    class port : public Clock {
    public:
        using register_type = uint32_t;

        static constexpr uintptr_t GPIOx_CRL = 0x00;
        static constexpr uintptr_t GPIOx_CRH = 0x04;
        static constexpr uintptr_t GPIOx_IDR = 0x08;
        static constexpr uintptr_t GPIOx_ODR = 0x0C;
        static constexpr uintptr_t GPIOx_BSRR = 0x10;
        static constexpr uintptr_t GPIOx_BRR = 0x14;
        static constexpr uintptr_t GPIOx_LCKR = 0x18;

        static constexpr uintptr_t address = Address;

        port() = delete;

        static inline register_type read() {
            return mem[GPIOx_IDR];
        }

        static inline void low(register_type mask) {
            mem[GPIOx_BRR] = mask;
        }

        static inline void high(register_type mask) {
            mem[GPIOx_BSRR] = mask;
        }

        static inline void toggle(register_type mask) {
            auto data = mem[GPIOx_ODR];
            mem[GPIOx_BRR] = data & mask;
            mem[GPIOx_BSRR] = ~data & mask;
        }

        template<::zoal::gpio::pin_mode PinMode>
        static inline void mode(register_type mask) {
            using namespace zoal::gpio;
            config_reg<pin_mode_to_cnf_mode<PinMode>::value>(mem[GPIOx_CRL], mask & 0xFF);
            config_reg<pin_mode_to_cnf_mode<PinMode>::value>(mem[GPIOx_CRH], mask >> 0x8);
            switch (PinMode) {
                case pin_mode::input_pull_down:
                    mem[GPIOx_BRR] = mask;
                    break;
                case pin_mode::input_pull_up:
                    mem[GPIOx_BSRR] = mask;
                    break;
                default:
                    break;
            }
        }

        template<::zoal::gpio::pin_mode PinMode, register_type mask>
        static inline void mode() {
            using namespace zoal::gpio;
            config_reg<pin_mode_to_cnf_mode<PinMode>::value>(mem[GPIOx_CRL], mask & 0xFF);
            config_reg<pin_mode_to_cnf_mode<PinMode>::value>(mem[GPIOx_CRH], mask >> 0x8);
            switch (PinMode) {
                case pin_mode::input_pull_down:
                    mem[GPIOx_BRR] = mask;
                    break;
                case pin_mode::input_pull_up:
                    mem[GPIOx_BSRR] = mask;
                    break;
                default:
                    break;
            }
        }
    private:
        static zoal::utils::memory_segment<uint32_t, Address> mem;

        template<uint32_t ConfigAndModeBits>
        static inline void config_reg(volatile register_type &reg, register_type mask) {
            using namespace ::zoal::gpio;
            if (mask == 0) {
                return;
            }

            register_type reset = 0;
            register_type set = 0;
            for (register_type i = 0; i < 8; i++) {
                if ((mask >> i) & 1) {
                    reset |= (0xF << (i << 2));
                    set |= ConfigAndModeBits << (i << 2);
                }
            }

            reg = (reg & ~reset) | set;
        }
    };

    template<uintptr_t Address, class Clock>
    zoal::utils::memory_segment<uint32_t, Address> port<Address, Clock>::mem;
}}}

#endif

#pragma once

#ifndef ZOAL_ARCH_STM32F1_PARALLEL_IO_CONTROLLER_HPP
#define ZOAL_ARCH_STM32F1_PARALLEL_IO_CONTROLLER_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace stm32f1 {
    template<uintptr_t Address, class Clock>
    class port : public Clock {
    public:
        using register_type = uint32_t;

        static constexpr uintptr_t CRL = 0x00;
        static constexpr uintptr_t CRH = 0x04;
        static constexpr uintptr_t IDR = 0x08;
        static constexpr uintptr_t ODR = 0x0C;
        static constexpr uintptr_t BSRR = 0x10;
        static constexpr uintptr_t BRR = 0x14;
        static constexpr uintptr_t LCKR = 0x18;

        static constexpr uintptr_t address = Address;

        port() = delete;

        static inline register_type read() {
            return mem[IDR];
        }

        static inline void low(register_type mask = ~(register_type) 0) {
            mem[BRR] = mask;
        }

        static inline void high(register_type mask = ~(register_type) 0) {
            mem[BSRR] = mask;
        }

        static inline void toggle(register_type mask = ~(register_type) 0) {
            auto data = mem[ODR];
            mem[BRR] = data & mask;
            mem[BSRR] = ~data & mask;
        }

        template<::zoal::gpio::pin_mode pm>
        static inline void mode(register_type mask) {
            using namespace zoal::gpio;
            configReg<MTB(pm)>(&mem[CRL], mask & 0xFF);
            configReg<MTB(pm)>(&mem[CRH], mask >> 0x8);
            switch (pm) {
                case pin_mode::input_pull_down:
                    mem[BRR] = mask;
                    break;
                case pin_mode::input_pull_up:
                    mem[BSRR] = mask;
                    break;
                default:
                    break;
            }
        }

        static inline void analog_mode(register_type mask) {
            configReg<0>(&mem[CRL], mask & 0xFF);
            configReg<0>(&mem[CRH], mask >> 0x8);
        }

        static inline void alternate_push_pullMode(register_type mask) {
            configReg<0x8 | 0x1>(&mem[CRL], mask & 0xFF);
            configReg<0x8 | 0x1>(&mem[CRH], mask >> 0x8);
        }

        static inline void alternate_open_drain_mode(register_type mask) {
            configReg<0xC | 0x1>(&mem[CRL], mask & 0xFF);
            configReg<0xC | 0x1>(&mem[CRH], mask >> 0x8);
        }

        static void mode(register_type mask, ::zoal::gpio::pin_mode pinMode) {
            using namespace zoal::gpio;
            switch (pinMode) {
                case pin_mode::input_floating:
                    mode<pin_mode::input_floating>(mask);
                    break;
                case pin_mode::input_pull_down:
                    mode<pin_mode::input_pull_down>(mask);
                    break;
                case pin_mode::input_pull_up:
                    mode<pin_mode::input_pull_up>(mask);
                    break;
                case pin_mode::output_open_drain:
                    mode<pin_mode::output_open_drain>(mask);
                    break;
                default:
                    mode<pin_mode::output_push_pull>(mask);
                    break;
            }
        }

    private:
        static zoal::utils::memory_segment<uint32_t, Address> mem;

        static constexpr uint32_t MTB(::zoal::gpio::pin_mode mode) {
            using namespace zoal::gpio;
            return (mode == pin_mode::input_floating) ? 0x4
                                                      : (mode == pin_mode::input_pull_down) ? 0x8
                                                                                            : (mode ==
                                                                                               pin_mode::input_pull_up)
                                                                                              ? 0x8
                                                                                              : (mode ==
                                                                                                 pin_mode::output_open_drain)
                                                                                                ? (0x4 | 0x1)
                                                                                                : (mode ==
                                                                                                   pin_mode::output_push_pull)
                                                                                                  ? (0x0 | 0x1) : 0;
        }

        template<uint32_t md>
        static inline void configReg(volatile register_type *reg, register_type mask) {
            using namespace ::zoal::gpio;
            if (mask == 0) {
                return;
            }

            register_type reset = 0;
            register_type set = 0;
            for (register_type i = 0; i < 8; i++) {
                if ((mask >> i) & 1) {
                    reset |= (0xF << (i << 2));
                    set |= md << (i << 2);
                }
            }

            *reg &= ~reset;
            *reg |= set;
        }
    };

    template<uintptr_t Address, class Clock>
    zoal::utils::memory_segment<uint32_t, Address> port<Address, Clock>::mem;
}}}

#endif

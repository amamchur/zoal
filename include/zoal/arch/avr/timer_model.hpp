
#ifndef ZOAL_GPIO_ATMEL_AVR_TIMER_MODEL_HPP
#define ZOAL_GPIO_ATMEL_AVR_TIMER_MODEL_HPP

#include <stdint.h> /* NOLINT */
#include "mcu_type.hpp"
#include "../../periph/timer_mode.hpp"
#include "../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    enum : uint8_t {
        WGMx0 = 0,
        WGMx1 = 1,
        WGMx2 = 3,
        WGMx3 = 4,
        cTCCRxA = static_cast<uint8_t>(~(1u << WGMx0 | 1u << WGMx1)),
        cTCCRxB = static_cast<uint8_t>(~(1u << WGMx2 | 1u << WGMx3))
    };

    template<uint8_t No, zoal::periph::timer_mode tm>
    struct timer8_mode {
        static_assert((No & 0u) != 0, "Unsupported timer mode");
    };

    template<uint8_t No>
    struct timer8_mode<No, zoal::periph::timer_mode::normal> {
        static constexpr uint8_t sTCCRxA = 0;
        static constexpr uint8_t sTCCRxB = 0;
    };

    template<uint8_t No>
    struct timer8_mode<No, zoal::periph::timer_mode::fast_pwm_8bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx0 | 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 0;
    };

    template<uint8_t No>
    struct timer8_mode<No, zoal::periph::timer_mode::phase_correct_8bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx0;
        static constexpr uint8_t sTCCRxB = 0;
    };

    template<uint8_t No, zoal::periph::timer_mode tm>
    struct timer_16_mode {
        static_assert((No & 0u) != 0, "Unsupported timer mode");
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::normal> {
        static constexpr uint8_t sTCCRxA = 0;
        static constexpr uint8_t sTCCRxB = 0;
        static constexpr uint16_t sICRx = 0xFFFF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::fast_pwm_8bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx0;
        static constexpr uint8_t sTCCRxB = 1u << WGMx2;
        static constexpr uint16_t sICRx = 0x00FF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::fast_pwm_9bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 1u << WGMx2;
        static constexpr uint16_t sICRx = 0xFFFF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::fast_pwm_10bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx0 | 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 1u << WGMx2;
        static constexpr uint16_t sICRx = 0x03FF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::fast_pwm_16bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 1u << WGMx2;
        static constexpr uint16_t sICRx = 0xFFFF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::phase_correct_8bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx0;
        static constexpr uint8_t sTCCRxB = 0;
        static constexpr uint16_t sICRx = 0x00FF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::phase_correct_9bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 1u << WGMx3;
        static constexpr uint16_t sICRx = 0x01FF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::phase_correct_10bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx0 | 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 0;
        static constexpr uint16_t sICRx = 0x01FF;
    };

    template<uint8_t No>
    struct timer_16_mode<No, zoal::periph::timer_mode::phase_correct_16bit> {
        static constexpr uint8_t sTCCRxA = 1u << WGMx1;
        static constexpr uint8_t sTCCRxB = 1u << WGMx3;
        static constexpr uint16_t sICRx = 0xFFFF;
    };

    template<mcu_type type>
    class timer8_model {
    };

    template<>
    class timer8_model<mcu_type::atmega> {
    public:
        using word = uint8_t;

        static constexpr uintptr_t TCCRxA = 0;
        static constexpr uintptr_t TCCRxB = 1;
        static constexpr uintptr_t TCNTx = 2;
        static constexpr uintptr_t OCRxA = 3;
        static constexpr uintptr_t OCRxB = 4;

        template<uint8_t No, zoal::periph::timer_mode TimerMode>
        using timer_mode = typename ::zoal::arch::avr::timer8_mode<No, TimerMode>;

        template<class T, zoal::periph::timer_mode TimerMode>
        static void mode() {
            zoal::utils::memory_segment<uint8_t, T::address> m8;
            m8[TCCRxA] = (m8[TCCRxA] & cTCCRxA) | timer_mode<T::no, TimerMode>::sTCCRxA;
            m8[TCCRxB] = (m8[TCCRxB] & cTCCRxB) | timer_mode<T::no, TimerMode>::sTCCRxB;
            m8.happyInspection();
        }
    };

    template<>
    class timer8_model<mcu_type::attiny13> {
    public:
        using word = uint8_t;

        static constexpr uintptr_t OCRxB = 0;
        static constexpr uintptr_t TCCRxA = 6;
        static constexpr uintptr_t TCNTx = 9;
        static constexpr uintptr_t TCCRxB = 10;
        static constexpr uintptr_t OCRxA = 13;

        template<uint8_t No, zoal::periph::timer_mode TimerMode>
        using timer_mode = typename ::zoal::arch::avr::timer8_mode<No, TimerMode>;

        template<class T, zoal::periph::timer_mode TimerMode>
        static void mode() {
            zoal::utils::memory_segment<uint8_t, T::address> m8;
            m8[TCCRxA] = (m8[TCCRxA] & cTCCRxA) | timer_mode<T::no, TimerMode>::sTCCRxA;
            m8[TCCRxB] = (m8[TCCRxB] & cTCCRxB) | timer_mode<T::no, TimerMode>::sTCCRxB;
            m8.happyInspection();
        }
    };

    template<>
    class timer8_model<mcu_type::attiny2313> {
    public:
        using word = uint8_t;

        static constexpr uintptr_t TCCRxA = 0;
        static constexpr uintptr_t TCNTx = 2;
        static constexpr uintptr_t TCCRxB = 3;
        static constexpr uintptr_t OCRxA = 6;
        static constexpr uintptr_t OCRxB = 12;

        template<uint8_t No, zoal::periph::timer_mode TimerMode>
        using timer_mode = typename ::zoal::arch::avr::timer8_mode<No, TimerMode>;

        template<class T, zoal::periph::timer_mode TimerMode>
        static void mode() {
            zoal::utils::memory_segment<uint8_t, T::address> m8;
            m8[TCCRxA] = (m8[TCCRxA] & cTCCRxA) | timer_mode<T::no, TimerMode>::sTCCRxA;
            m8[TCCRxB] = (m8[TCCRxB] & cTCCRxB) | timer_mode<T::no, TimerMode>::sTCCRxB;
            m8.happyInspection();
        }
    };

    template<mcu_type type>
    class timer16_model {
    };

    template<>
    class timer16_model<mcu_type::atmega> {
    public:
        using word = uint16_t;

        static constexpr uintptr_t TCCRxA = 0;
        static constexpr uintptr_t TCCRxB = 1;
        static constexpr uintptr_t TCCRxC = 2;

        static constexpr uintptr_t TCNTx = 4;
        static constexpr uintptr_t ICRx = 6;
        static constexpr uintptr_t OCRxA = 8;
        static constexpr uintptr_t OCRxB = 10;

        template<uint8_t No, zoal::periph::timer_mode tm>
        using timer_mode = typename ::zoal::arch::avr::timer_16_mode<No, tm>;

        template<class T, zoal::periph::timer_mode TimerMode>
        static inline void mode() {
            zoal::utils::memory_segment<uint8_t, T::address> m8;
            m8[TCCRxA] = (m8[TCCRxA] & cTCCRxA) | timer_mode<T::no, TimerMode>::sTCCRxA;
            m8[TCCRxB] = (m8[TCCRxB] & cTCCRxB) | timer_mode<T::no, TimerMode>::sTCCRxB;
            m8.happyInspection();

            zoal::utils::memory_segment<uint16_t, T::address> m16;
            m16[ICRx] = timer_mode<T::No, TimerMode>::sICRx;
            m16.happyInspection();
        }
    };

    template<>
    class timer16_model<mcu_type::attiny2313> {
    public:
        using word = uint16_t;

        static constexpr uintptr_t ICRx = 0;
        static constexpr uintptr_t OCRxB = 4;
        static constexpr uintptr_t OCRxA = 6;
        static constexpr uintptr_t TCNTx = 8;
        static constexpr uintptr_t TCCRxB = 10;
        static constexpr uintptr_t TCCRxA = 11;

        template<uint8_t No, zoal::periph::timer_mode tm>
        using timer_mode = typename ::zoal::arch::avr::timer_16_mode<No, tm>;

        template<class T, zoal::periph::timer_mode TimerMode>
        static void mode() {
            zoal::utils::memory_segment<uint8_t, T::address> m8;
            m8[TCCRxA] = (m8[TCCRxA] & cTCCRxA) | timer_mode<T::no, TimerMode>::sTCCRxA;
            m8[TCCRxB] = (m8[TCCRxB] & cTCCRxB) | timer_mode<T::no, TimerMode>::sTCCRxB;
            m8.happyInspection();

            zoal::utils::memory_segment<uint16_t, T::address> m16;
            m16[ICRx] = timer_mode<T::No, TimerMode>::sICRx;
            m16.happyInspection();
        }
    };
}}}

#endif

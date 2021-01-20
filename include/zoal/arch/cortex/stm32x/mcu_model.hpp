#ifndef ZOAL_MCU_MODEL_HPP
#define ZOAL_MCU_MODEL_HPP

namespace zoal { namespace arch { namespace stm32x {
    enum class stm32_type {
        //
        mainstream = 'F',
        low_power = 'L',
        high_performance = 'H',
        wireless = 'W'
    };

    enum class stm32_core {
        //
        m0_0 = '0', // STM32F030
        m3_1 = '1', // STM32F103
        m3_2 = '2',
        m4_3 = '3',
        m4_4 = '4',
        m7_7 = '7'
    };

    enum class stm32_no_of_pins {
        //
        f = 20,
        g = 28, // STM32F103
        k = 32,
        t = 36,
        s = 44,
        c = 48,
        r = 64,
        v = 100,
        z = 144,
        i = 176
    };

    template<stm32_type Type, stm32_core Core, stm32_no_of_pins Pins>
    class model {
    public:
        static constexpr stm32_type type = Type;
        static constexpr stm32_core core = Core;
        static constexpr stm32_no_of_pins pins = Pins;
    };
}}}

#endif

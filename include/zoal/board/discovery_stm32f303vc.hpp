#ifndef ZOAL_DISCOVERY_STM32F303VC_HPP
#define ZOAL_DISCOVERY_STM32F303VC_HPP

#include "../gpio/api.hpp"
#include "../io/button.hpp"
#include "../mcu/stm32f303vctx.hpp"

namespace zoal { namespace board {
    class discovery_stm32f303vc {
    public:
        using mcu = zoal::mcu::stm32f303vctx;

        using led_03 = zoal::gpio::active_high<mcu::pe_09>;
        using led_04 = zoal::gpio::active_high<mcu::pe_08>;
        using led_05 = zoal::gpio::active_high<mcu::pe_10>;
        using led_06 = zoal::gpio::active_high<mcu::pe_15>;
        using led_07 = zoal::gpio::active_high<mcu::pe_11>;
        using led_08 = zoal::gpio::active_high<mcu::pe_14>;
        using led_09 = zoal::gpio::active_high<mcu::pe_12>;
        using led_10 = zoal::gpio::active_high<mcu::pe_13>;
        using led_gpio_cfg = zoal::gpio::api::optimize<
            //
            led_03::gpio_cfg,
            led_04::gpio_cfg,
            led_05::gpio_cfg,
            led_06::gpio_cfg,
            led_07::gpio_cfg,
            led_08::gpio_cfg,
            led_09::gpio_cfg,
            led_10::gpio_cfg>;

        template<class TimeType>
        using user_button_type = zoal::io::button<TimeType, mcu::pa_00, zoal::io::active_high_button>;
    };
}}

#endif

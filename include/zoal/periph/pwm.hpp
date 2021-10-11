#ifndef ZOAL_PWM_HPP
#define ZOAL_PWM_HPP

namespace zoal { namespace periph {
    template<class Timer, class Pin, uint8_t Channel, class OCReg, class Connect, class Disconnect>
    class pwm_channel {
    public:
        using timer = Timer;
        using pin = Pin;
        using ocr = OCReg;
        static constexpr auto channel = Channel;
        using connect = Connect;
        using disconnect = Disconnect;

        inline static void set(typename ocr::type value) {
            ocr::ref() = value;
        }
    };
}}

#endif

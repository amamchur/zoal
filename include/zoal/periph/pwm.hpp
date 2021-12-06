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

        inline static typename ocr::type& pulse_width() {
            return ocr::ref();
        }
    };

    class null_pwm_channel {
    public:
        inline static void set(size_t value) {
        }

        inline static size_t pulse_width() {
            return 0;
        }
    };
}}

#endif

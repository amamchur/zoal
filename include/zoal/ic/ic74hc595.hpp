#ifndef ZOAL_IC_IC74HC595_HPP
#define ZOAL_IC_IC74HC595_HPP

#include "../gpio/pin_mode.hpp"
#include "../gpio/shift_out.hpp"

namespace zoal { namespace ic {
    template<class Tools, class SerialDataInput, class StorageRegisterClock, class ShiftRegisterClock>
    class ic74hc595 {
    public:
        using tools = Tools;
        using api = typename tools::api;
        using self_type = ic74hc595<Tools, SerialDataInput, StorageRegisterClock, ShiftRegisterClock>;
        using gpio_cfg = typename api::template mode<zoal::gpio::pin_mode::output_push_pull,
                                                     SerialDataInput,
                                                     StorageRegisterClock,
                                                     ShiftRegisterClock>;

        class transaction {
        public:
            inline transaction() {
                StorageRegisterClock::high();
            }

            transaction(const transaction &) = delete;

            template<class T>
            transaction &operator<<(T data) {
                ::zoal::gpio::shift_out_msbf<SerialDataInput, ShiftRegisterClock>(data);
                return *this;
            }

            inline ~transaction() {
                StorageRegisterClock::low();
            }
        };

        ic74hc595() = delete;

        static void write(uint8_t value) {
            if (value) {
                high();
            } else {
                low();
            }
        }

        static void low() {
            transaction t;
            SerialDataInput::low();
            ShiftRegisterClock::low();
            ShiftRegisterClock::high();
        }

        static void high() {
            transaction t;
            SerialDataInput::high();
            ShiftRegisterClock::low();
            ShiftRegisterClock::high();
        }
    };
}}

#endif

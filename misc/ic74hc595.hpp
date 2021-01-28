#ifndef ZOAL_IC_IC74HC595_HPP
#define ZOAL_IC_IC74HC595_HPP

#include <zoal/gpio/api.hpp>
#include <zoal/gpio/shift_out.hpp>

namespace zoal { namespace ic {
    template<class Tools, class SerialDataInput, class ShiftRegisterClock, class StorageRegisterClock>
    class ic74hc595 {
    public:
        using tools = Tools;
        using self_type = ic74hc595<Tools, SerialDataInput, ShiftRegisterClock, StorageRegisterClock>;
        using gpio_cfg = zoal::gpio::api::optimize<
            //
            zoal::gpio::api::mode<
                //
                zoal::gpio::pin_mode::output_push_pull,
                SerialDataInput,
                ShiftRegisterClock,
                StorageRegisterClock>,
            zoal::gpio::api::low<SerialDataInput, ShiftRegisterClock, StorageRegisterClock>>;

        class transaction {
        public:
            transaction() {
                typename StorageRegisterClock::high();
            }

            transaction(const transaction &) = delete;
            transaction(const transaction &&) = delete;

            template<class T>
            transaction &operator<<(T data) {
                ::zoal::gpio::shift_out_msbf<SerialDataInput, ShiftRegisterClock>(data);
                return *this;
            }

            ~transaction() {
                typename StorageRegisterClock::low();
            }
        };
    };
}}

#endif

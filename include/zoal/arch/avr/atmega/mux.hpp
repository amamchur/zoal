#ifndef ZOAL_ARCH_AVR_MUX_HPP
#define ZOAL_ARCH_AVR_MUX_HPP

#include "../../../ct/check.hpp"
#include "../../../gpio/api.hpp"
#include "../../../gpio/pin.hpp"
#include "../../../utils/helpers.hpp"

namespace zoal { namespace metadata {
    template<class Sign, uintptr_t UsartAddress, uint32_t PortAddress, uint8_t PinOffset>
    struct usart_mapping;

    template<class Sign, uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct spi_mapping;

    template<class Sign, uintptr_t Address, uint32_t Port, uint8_t PinOffset>
    struct i2c_mapping;

    template<class Sign, class Adc, class Pin>
    struct adc_mapping;

    template<class Sign, class Timer, class Pin, uint8_t Channel>
    struct pwm_channel_mapping;
}}

namespace zoal { namespace arch { namespace avr { namespace atmega {
    using zoal::ct::type_list;
    using zoal::gpio::api;
    using zoal::gpio::pin_mode;
    using zoal::metadata::adc_mapping;
    using zoal::metadata::i2c_mapping;
    using zoal::metadata::pwm_channel_mapping;
    using zoal::metadata::spi_mapping;
    using zoal::metadata::usart_mapping;

    template<class Microcontroller>
    class mux {
    public:
        using mcu = Microcontroller;
        using sign = typename mcu::signature;

        template<class U, class Rx, class Tx, class Ck = zoal::gpio::null_pin>
        class usart {
        public:
            using self_type = usart<U, Rx, Tx, Ck>;

            using rxm = usart_mapping<sign, U::address, Rx::port::address, Rx::offset>;
            using txm = usart_mapping<sign, U::address, Tx::port::address, Tx::offset>;
            using ckm = usart_mapping<sign, U::address, Ck::port::address, Ck::offset>;

            static_assert(rxm::rx >= 0, "Specified RX pin could not be connected to USART");
            static_assert(txm::tx >= 0, "Specified TX pin could not be connected to USART");
            static_assert(Ck::not_pin || ckm::ck >= 0, "Specified CX pin could not be connected to USART");

            using connect = zoal::ct::type_list<zoal::mem::null_cas>;
            using disconnect = zoal::ct::type_list<zoal::mem::null_cas>;
        };

        template<class A, class Pin>
        class adc {
        public:
            static constexpr auto channel = adc_mapping<sign, A, Pin>::value;
            static constexpr auto mux_set_mask = channel & 0x07;
            static constexpr auto mux5 = channel > 7 ? (1 << 3) : 0;

            static_assert(channel >= 0, "Specified pin could not be connected to ADC");

            using connect = zoal::mem::callable_cas_list_variadic<
                //
                typename A::ADMUXx::template cas<0x0F, mux_set_mask>,
                typename A::ADCSRBx::template cas<(1 << 3), mux5>>;
            using disconnect = zoal::mem::null_cas_list;
        };

        template<class T, class Pin, uint8_t Channel>
        class timer {
        public:
            static_assert(Channel < 2, "Channel index if out of range");
            static_assert(pwm_channel_mapping<sign, T, Pin, Channel>::value, "Unsupported PWM connection");

            static constexpr auto pwm_mode = 1;
            static constexpr auto mask_shift = Channel == 0 ? 7 : 5;
            static constexpr auto clear_mask = Channel == 0 ? T::TCCRxA_COMxA : T::TCCRxA_COMxB;
            static constexpr auto set_mask = static_cast<uint8_t>(pwm_mode << mask_shift);
            static_assert((set_mask & clear_mask) == set_mask, "Ops, wrong metadata");

            using connect = zoal::mem::callable_cas_list_variadic<typename T::TCCRxA::template cas<clear_mask, set_mask>>;
            using disconnect = zoal::mem::callable_cas_list_variadic<typename T::TCCRxA::template cas<clear_mask, 0>>;
        };

        template<class S, class Mosi, class Miso, class Clock, class SlaveSelect>
        class spi {
        public:
            using mosi = spi_mapping<sign, S::address, Mosi::port::address, Mosi::offset>;
            using miso = spi_mapping<sign, S::address, Miso::port::address, Miso::offset>;
            using clock = spi_mapping<sign, S::address, Clock::port::address, Clock::offset>;
            using ss = spi_mapping<sign, S::address, SlaveSelect::port::address, SlaveSelect::offset>;

            static_assert(mosi::mosi >= 0, "Specified MOSI pin could not be connected to SPI");
            static_assert(miso::miso >= 0, "Specified MISO pin could not be connected to SPI");
            static_assert(clock::clock >= 0, "Specified CLK pin could not be connected to SPI");
            static_assert(ss::slave_select >= 0, "Specified SS pin could not be connected to SPI");

            using connect = typename api::optimize<
                //
                api::mode<pin_mode::output, Mosi, Clock, SlaveSelect>,
                api::mode<pin_mode::input, Miso>>;
            using disconnect = typename api::optimize<
                //
                api::mode<pin_mode::input, Mosi, Miso, Clock, SlaveSelect>>;
        };

        template<class I, class SerialDataLine, class SerialClockLine>
        class i2c {
        public:
            using sda = i2c_mapping<sign, I::address, SerialDataLine::port::address, SerialDataLine::offset>;
            using scl = i2c_mapping<sign, I::address, SerialClockLine::port::address, SerialClockLine::offset>;

            static_assert(sda::sda >= 0, "Specified SDA pin could not be connected to I2C");
            static_assert(scl::scl >= 0, "Specified SCL pin could not be connected to I2C");

            using connect = typename api::optimize<
                //
                api::mode<pin_mode::output_push_pull, SerialDataLine, SerialClockLine>,
                api::high<SerialDataLine, SerialClockLine>>;
            using disconnect = typename api::optimize<
                //
                api::mode<pin_mode::input_floating, SerialDataLine, SerialClockLine>>;
        };
    };
}}}}

#endif

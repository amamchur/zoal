#ifndef ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP
#define ZOAL_ARCH_ATMEL_AVR_ATMEGA_HARDWARE_SPI_HPP

#include "../../../gpio/pin_mode.hpp"
#include "../../../utils/memory_segment.hpp"

namespace zoal { namespace arch { namespace avr {
    template<class MOSI, class MISO, class SCLK, class SS>
    class base_spi {
    public:
        enum : uint8_t {
            SPIFx = 7,
            SPEx = 6,
            MSTRx = 4,
            DORDx = 5,
            SPI2Xx = 0,
        };

        typedef base_spi<MISO, MISO, SCLK, SS> Class;

        static constexpr uintptr_t SPCRx = 0;
        static constexpr uintptr_t SPSRx = 1;
        static constexpr uintptr_t SPDRx = 2;

        base_spi() = delete;

        base_spi(const base_spi &) = delete;

        static uint8_t inline transfer_byte(uint8_t data) {
            zoal::utils::memory_segment<uint8_t, 0x4C> m8;
            m8[SPDRx] = data;

            asm volatile("nop");

            while (!(m8[SPSRx] & (1u << SPIFx)));
            return m8[SPDRx];
        }

        static void begin() {
            using namespace zoal::gpio;
            MOSI::template mode<pin_mode::output>();
            SCLK::template mode<pin_mode::output>();
            SS::template mode<pin_mode::output>();
            SS::high();
        }
    };

    template<class MOSI, class MISO, class SCLK, class SS, uint8_t Mode>
    class msbf_spi : public base_spi<MOSI, MISO, SCLK, SS> {
    public:
        typedef base_spi<MOSI, MISO, SCLK, SS> Base;

        template<class T>
        static T inline transfer(T data) {
            T result = 0;
            uint8_t i = sizeof(T) * 8;
            do {
                i -= 8;
                result |= Base::transfer_byte((data >> i) & 0xFF) << i;
            } while (i != 0);
            return result;
        }

        static void setup() {
            zoal::utils::memory_segment<uint8_t, 0x4C> m8;
            m8[Base::SPCRx] = static_cast<uint8_t>(1 << Base::SPEx | 1 << Base::MSTRx | Mode);
            m8[Base::SPSRx] = 1 << Base::SPI2Xx;
        }
    };

    template<class MOSI, class MISO, class SCLK, class SS, uint8_t Mode>
    class lsbf_spi : public base_spi<MOSI, MISO, SCLK, SS> {
    public:
        typedef base_spi<MOSI, MISO, SCLK, SS> Base;

        static void setup() {
            zoal::utils::memory_segment<uint8_t, 0x4C> m8;
            m8[Base::SPCRx] = static_cast<uint8_t>(1 << Base::SPEx | 1 << Base::MSTRx | 1 << Base::DORDx | Mode);
            m8[Base::SPSRx] = 1 << Base::SPI2Xx;
        }

        template<class T>
        static T inline transfer(T data) {
            T result = 0;
            uint8_t i = 0;
            uint8_t j = sizeof(T) * 8;
            do {
                i += 8;
                result = Base::transfer_byte((data >> i) & 0xFF) << i;
            } while (i != j);
            return result;
        }
    };
}}}

#endif

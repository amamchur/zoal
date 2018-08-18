#ifndef ZOAL_IC_WS2812B_HPP
#define ZOAL_IC_WS2812B_HPP

#include <stddef.h>
#include "../gpio/pin_mode.hpp"
#include "../utils/nop.hpp"
#include "../utils/interrupts.hpp"

namespace zoal { namespace ic {
    class timing_ws2812 {
    public:
        static constexpr uint32_t th_0_ns = 350;
        static constexpr uint32_t tl_0_ns = 800;
        static constexpr uint32_t th_1_ns = 700;
        static constexpr uint32_t tl_1_ns = 600;
        static constexpr uint32_t reset_us = 50;
    };

    class timing_ws2812b {
    public:
        static constexpr uint32_t th_0_ns = 400;
        static constexpr uint32_t tl_0_ns = 800;
        static constexpr uint32_t th_1_ns = 850;
        static constexpr uint32_t tl_1_ns = 450;
        static constexpr uint32_t reset_us = 50;
    };

    class timing_sk6812 {
    public:
        static constexpr uint32_t th_0_ns = 300;
        static constexpr uint32_t tl_0_ns = 900;
        static constexpr uint32_t th_1_ns = 600;
        static constexpr uint32_t tl_1_ns = 600;
        static constexpr uint32_t reset_us = 80;
    };

    template<uint32_t Frequency, class Timing>
    class ws2812_code_cycles {
    public:
        static constexpr float ns_per_cycle = 1000.0f / (Frequency / 1000000);
        static constexpr int32_t th_0_ns = Timing::th_0_ns / ns_per_cycle;
        static constexpr int32_t tl_0_ns = Timing::tl_0_ns / ns_per_cycle;
        static constexpr int32_t tl_1_ns = Timing::tl_1_ns / ns_per_cycle;
        static constexpr int32_t th_1_ns = Timing::th_1_ns / ns_per_cycle;
    };

    template<class DataPin, uint32_t Frequency, class Timing = timing_ws2812, int8_t OutputOverhead = 2>
    class transmitter_ws2812 {
    public:
        using data_pin = DataPin;
        using timing = Timing;
        using cycles = ws2812_code_cycles<Frequency, timing>;
        using port = typename DataPin::port;

        static constexpr int8_t out_overhead = OutputOverhead;    // Mostly for address load
        static constexpr int8_t call_overhead = 4;

        //
        // __attribute__((noinline)) - is essential to avoid
        // branching optimization in inline mode
        //
        static __attribute__((noinline)) void zero() {
            using namespace zoal::utils;

            data_pin::high();
            nop<cycles::th_0_ns - out_overhead>::place();

            data_pin::low();
            nop<cycles::tl_0_ns - out_overhead - call_overhead>::place();
        }

        static __attribute__((noinline)) void one() {
            using namespace zoal::utils;

            data_pin::high();
            nop<cycles::th_1_ns - out_overhead>::place();

            data_pin::low();
            nop<cycles::tl_1_ns - out_overhead - call_overhead>::place();
        }
    };

    template<class Transmitter, class Delay>
    class ws2812 {
    public:
        using delay = Delay;
        using transmitter = Transmitter;
        using data_pin = typename transmitter::data_pin;
        using timing = typename transmitter::timing;

        static void begin() {
            using namespace zoal::gpio;
            data_pin::template mode<pin_mode::output>();
            data_pin::low();
        }

        static void send_byte(uint8_t value) {
            for (uint8_t i = 0; i < 8; i++) {
                if ((value & 0x08u) == 0) {
                    transmitter::zero();
                } else {
                    transmitter::one();
                }
                value <<= 1;
            }
        }

        template<class Pixel>
        static void send(const Pixel *data, int count) {
            using namespace zoal::utils;
            for (int i = 0; i < count; i++) {
                interrupts ni(false);
                const auto *pixel = (const uint8_t *) (data + i);
                for (size_t j = 0; j < sizeof(Pixel); j++, pixel++) {
                    send_byte(*pixel);
                }
            }
            delay::template us<timing::reset_us>();
        }
    };
}}

#endif

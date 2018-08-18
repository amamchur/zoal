#pragma once

namespace Tests { namespace Utils {
    template<uint64_t mcuFreq, uint64_t count>
    struct NoopCycles {
        static constexpr int64_t NanoSecPerClock = (1000000 / (mcuFreq / 1000));
        static int64_t Overhead;

        static inline void place() {
            auto start = std::chrono::high_resolution_clock::now();
            decltype(start) finish;
            auto pause = NanoSecPerClock * count - Overhead;
            long long diff = 0;
            while (diff < pause) {
                finish = std::chrono::high_resolution_clock::now();
                diff = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            }
        }

        static void measureOverhead() {
            auto start = std::chrono::high_resolution_clock::now();
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

            const int times = 10;
            long sum = 0;
            for (int i = 0; i < times; i++) {
                start = std::chrono::high_resolution_clock::now();
                finish = std::chrono::high_resolution_clock::now();
                sum += std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            }

            Overhead = sum / times;
        }

        static void printOverhead() {
            auto start = std::chrono::high_resolution_clock::now();
            auto finish = std::chrono::high_resolution_clock::now();
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            std::cout << ns << "ns\n";
        }
    };

    template<uint64_t mcuFreq, uint64_t count>
    int64_t NoopCycles<mcuFreq, count>::Overhead = 0;
}}
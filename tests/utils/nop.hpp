#ifndef ZOAL_TEST_UTILS_NOP_HPP
#define ZOAL_TEST_UTILS_NOP_HPP

#include <chrono>
#include <iostream>
#include <stdint.h>

namespace tests { namespace utils {
    template<uint64_t Frequency, uint64_t Count>
    struct nop {
        static constexpr int64_t ns_per_clock = (1000000 / (Frequency / 1000));
        static int64_t overhead;

        static inline void place() {
            auto start = std::chrono::high_resolution_clock::now();
            decltype(start) finish;
            auto pause = ns_per_clock * Count - overhead;
            long long diff = 0;
            while (diff < pause) {
                finish = std::chrono::high_resolution_clock::now();
                diff = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            }
        }

        static void measure_overhead() {
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

            overhead = sum / times;
        }

        static void print_overhead() {
            auto start = std::chrono::high_resolution_clock::now();
            auto finish = std::chrono::high_resolution_clock::now();
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
            std::cout << ns << "ns\n";
        }
    };

    template<uint64_t mcuFreq, uint64_t count>
    int64_t nop<mcuFreq, count>::overhead = 0;
}}

#endif

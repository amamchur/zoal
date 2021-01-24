#ifndef ZOAL_PROCESS_SYNC_I2C_MOCK_HPP
#define ZOAL_PROCESS_SYNC_I2C_MOCK_HPP

#include <chrono>
#include <thread>

#include <zoal/periph/i2c_request.hpp>

namespace zoal { namespace tests {
    template<class TestCase>
    class process_sync_i2c_mock {
    public:
        using test_case = TestCase;
        static zoal::periph::i2c_request request;
        static std::thread thread;

        static void join() {
            if (thread.joinable()) {
                thread.join();
            }
        }

        static void start() {
            join();

            thread = std::thread([]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                if (request.request_next()) {
                    request.enqueue(0xE7);
                }
                if (request.request_next()) {
                    request.enqueue(0x00);
                }

                request.complete(zoal::periph::i2c_result::ok);
            });
        }
    };

    template<class TestCase>
    zoal::periph::i2c_request process_sync_i2c_mock<TestCase>::request;

    template<class TestCase>
    std::thread process_sync_i2c_mock<TestCase>::thread;
}}

#endif

#include "../test_utils/i2c_mock.hpp"

#include "gtest/gtest.h"
#include <thread>
#include <zoal/periph/i2c_request.hpp>
#include <zoal/periph/i2c_request_dispatcher.hpp>
#include <zoal/utils/i2c_scanner.hpp>

TEST(i2c_scanner, scan_request) {
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;

    zoal::utils::i2c_scanner scanner;

    std::vector<uint8_t> found_devs;
    scanner.device_found.assign([&](uint8_t dev) { found_devs.push_back(dev); });
    scanner.scan(dispatcher)([&](int code) { finished = code == 0; });

    EXPECT_EQ(request.address_rw(), 0x2);
    EXPECT_EQ(request.ptr, nullptr);
    EXPECT_EQ(request.end - request.ptr, 0);

    uint8_t dev_id = 0x1F;
    while (request.status != zoal::periph::i2c_request_status::idle) {
        bool found = request.address_rw() == (dev_id << 1);
        if (found) {
            request.status = zoal::periph::i2c_request_status::finished;
        } else {
            request.status = zoal::periph::i2c_request_status::failed;
        }

        dispatcher.handle();
    }

    ASSERT_EQ(found_devs.size(), 1);
    EXPECT_EQ(found_devs[0], dev_id);
}

TEST(i2c_scanner, handle_until_finished) {
    using dispatcher_type = zoal::periph::i2c_request_dispatcher<zoal::tests::i2c_mock<typeof(this)>, sizeof(void *) * 8>;
    dispatcher_type dispatcher;
    zoal::periph::i2c_request &request = dispatcher.request;
    volatile bool finished = false;
    zoal::utils::i2c_scanner scanner;

    bool dev_0x48_found = false;
    scanner.device_found = [&](uint8_t addr) { dev_0x48_found = dev_0x48_found || addr == 0x48; };
    scanner.scan(dispatcher)([&](int code) { finished = code == 0; });

    EXPECT_FALSE(finished);
    EXPECT_FALSE(dev_0x48_found);

    volatile bool run = true;
    auto thread = std::thread([&run, &request]() {
        while (run) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            if (request.status == zoal::periph::i2c_request_status::pending) {
                auto addr = request.address_rw() >> 1;
                if (addr == 0x48) {
                    request.complete(zoal::periph::i2c_request_status::finished);
                } else {
                    request.complete(zoal::periph::i2c_request_status::failed);
                }
            }
        }
    });

    EXPECT_FALSE(finished);
    EXPECT_FALSE(dev_0x48_found);
    dispatcher.handle_until_finished();
    EXPECT_TRUE(finished);
    EXPECT_TRUE(dev_0x48_found);

    run = false;
    thread.join();
}

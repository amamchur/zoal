#include "gtest/gtest.h"
#include <zoal/periph/i2c_request.hpp>
#include <zoal/utils/i2c_scanner.hpp>

TEST(i2c_scanner, scan_request) {
    zoal::periph::i2c_request request;
    zoal::utils::i2c_scanner scanner;
    auto callback = [&](uint8_t addr){

    };
    scanner.scan(request);

    EXPECT_EQ(request.initiator, &scanner);
    EXPECT_EQ(request.address(), 0x2);
    EXPECT_EQ(request.ptr, nullptr);
    EXPECT_EQ(request.end - request.ptr, 0);

    auto res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    request.result = zoal::periph::i2c_result::pending;
    res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    request.result = zoal::periph::i2c_result::idle;
    res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    zoal::periph::i2c_request request2;
    res = scanner.complete_request(request2, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::ignored);

    request.result = zoal::periph::i2c_result::ok;
    res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::new_request);

    scanner.scan(request);
    request.result = zoal::periph::i2c_result::error;
    res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::new_request);

    scanner.scan(request);
    request.result = zoal::periph::i2c_result::end_of_stream;
    res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::new_request);

    scanner.scan(request);
    request.result = zoal::periph::i2c_result::ok;
    request.token = 0xFF;
    res = scanner.complete_request(request, callback);
    EXPECT_EQ(res,  zoal::periph::i2c_request_completion_result::finished);
}

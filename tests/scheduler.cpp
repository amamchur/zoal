#include "gtest/gtest.h"

#include <iostream>
#include <type_traits>
#include <zoal/mcu/atmega_48_88_168_328.hpp>

#include "utils/ms_counter.hpp"

using mcu = zoal::mcu::atmega_48_88_168_328<16000000>;
using counter = tests::utils::ms_counter;
using tools = zoal::utils::tool_set<mcu, counter>;

namespace {
    volatile bool run = false;
    volatile uint32_t start;
    volatile uint32_t finish;
    volatile int token;

    void handler() {
        finish = counter::now();
        run = false;
    }

    void handler_none() {
    }

    void handler_none_int(int) {
    }

    void handler_int(int t) {
        finish = counter::now();
        token = t;
        run = false;
    }

    class Cls {
    public:
        void handler() {
            finish = counter::now();
            run = false;
        }

        void handler_none() {
        }

        void handler_none_int(int) {
        }

        void handler_int(int t) {
            finish = counter::now();
            token = t;
            run = false;
        }
    };
}

using function_scheduler = tools::function_scheduler<4>;
using function_scheduler_int = tools::function_scheduler<4, int>;
using method_scheduler = tools::method_scheduler<Cls, 4>;
using method_scheduler_int = tools::method_scheduler<Cls, 4, int>;

TEST(function_scheduler, should_return_false_when_limit_exceeded) {
    function_scheduler scheduler;

    bool r = scheduler.schedule(0, handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(1, handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(2, handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(3, handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(4, handler);
    EXPECT_FALSE(r);
    r = scheduler.schedule(5, handler);
    EXPECT_FALSE(r);
    r = scheduler.schedule(6, handler);
    EXPECT_FALSE(r);

    scheduler.clear();
    r = scheduler.schedule(7, handler);
    EXPECT_TRUE(r);
}

TEST(function_scheduler, should_invoke_function_callback_without_token) {
    function_scheduler scheduler;

    scheduler.schedule(1, handler_none);
    scheduler.schedule(10, handler);
    scheduler.schedule(2, handler_none);

    run = true;
    start = counter::now();

    while (run) {
        scheduler.handle();
    }

    auto diff = finish - start;
    EXPECT_EQ(diff, 10);
}

TEST(function_scheduler, should_invoke_function_callback_with_token) {
    function_scheduler_int scheduler;

    run = true;
    start = counter::now();
    token = -1;

    scheduler.schedule(1, handler_none_int, 1234);
    scheduler.schedule(10, handler_int, 1234);
    scheduler.schedule(2, handler_none_int, 1234);

    while (run) {
        scheduler.handle();
    }

    {
        auto diff = finish - start;
        EXPECT_EQ(diff, 10);
        EXPECT_EQ(token, 1234);
    }

    run = true;
    start = counter::now();
    token = -1;
    scheduler.schedule(10, handler_int);

    while (run) {
        scheduler.handle();
    }

    {
        auto diff = finish - start;
        EXPECT_EQ(diff, 10);
        EXPECT_NE(token, -1);
    }
}


TEST(method_scheduler, should_return_false_when_limit_exceeded) {
    Cls cls;
    method_scheduler scheduler(&cls);

    bool r = scheduler.schedule(0, &Cls::handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(1, &Cls::handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(2, &Cls::handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(3, &Cls::handler);
    EXPECT_TRUE(r);
    r = scheduler.schedule(4, &Cls::handler);
    EXPECT_FALSE(r);
    r = scheduler.schedule(5, &Cls::handler);
    EXPECT_FALSE(r);
    r = scheduler.schedule(6, &Cls::handler);
    EXPECT_FALSE(r);

    scheduler.clear();
    r = scheduler.schedule(7, &Cls::handler);
    EXPECT_TRUE(r);
}

TEST(method_scheduler, should_invoke_function_callback_without_token) {
    Cls cls;
    method_scheduler scheduler(&cls);

    scheduler.schedule(1, &Cls::handler_none);
    scheduler.schedule(10, &Cls::handler);
    scheduler.schedule(2, &Cls::handler_none);

    run = true;
    start = counter::now();

    while (run) {
        scheduler.handle();
    }

    auto diff = finish - start;
    EXPECT_EQ(diff, 10);
}


TEST(method_scheduler, should_invoke_function_callback_with_token) {
    Cls cls;
    method_scheduler_int scheduler(&cls);

    run = true;
    start = counter::now();
    token = -1;
    scheduler.schedule(1, &Cls::handler_none_int, 1234);
    scheduler.schedule(10, &Cls::handler_int, 1234);
    scheduler.schedule(2, &Cls::handler_none_int, 1234);

    while (run) {
        scheduler.handle();
    }

    {
        auto diff = finish - start;
        EXPECT_EQ(diff, 10);
        EXPECT_EQ(token, 1234);
    }

    run = true;
    start = counter::now();
    token = -1;
    scheduler.schedule(10, &Cls::handler_int);

    while (run) {
        scheduler.handle();
    }

    {
        auto diff = finish - start;
        EXPECT_EQ(diff, 10);
        EXPECT_NE(token, -1);
    }
}

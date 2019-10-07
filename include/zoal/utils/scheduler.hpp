#ifndef ZOAL_UTILS_SCHEDULER_HPP
#define ZOAL_UTILS_SCHEDULER_HPP

#include <stdint.h>
#include <stddef.h>

namespace zoal { namespace utils {
    template<class CounterValueType, class Callback, class Token>
    class schedule_item {
    public:
        CounterValueType time;
        Callback handler;
        Token token;

        bool match(const schedule_item &item) const {
            return handler == item.handler && token == item.token;
        }

        schedule_item() = default;

        schedule_item(CounterValueType tm, Callback c)
                : time(tm), handler(c) {
        }

        schedule_item(CounterValueType tm, Callback c, Token t)
                : time(tm), handler(c), token(t) {
        }
    };

    template<class CounterValueType, class Callback>
    class schedule_item<CounterValueType, Callback, void> {
    public:
        CounterValueType time;
        Callback handler;

        bool match(const schedule_item &item) const {
            return handler == item.handler;
        }

        schedule_item() = default;

        schedule_item(CounterValueType tm, Callback c)
                : time(tm), handler(c) {
        }
    };

    template<class Counter, size_t Capacity, class Callback, class Token>
    class base_scheduler {
    public:
        using token_type = Token;
        using counter_type = typename Counter::value_type;
        using item_type = schedule_item<counter_type, Callback, token_type>;

        base_scheduler() noexcept : prev_time(Counter::now()) {
            clear();
        }

        template<typename... Args>
        bool schedule(counter_type dt, Args... args) {
            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (item.handler == nullptr) {
                    item = item_type(dt, args...);
                    return true;
                }
            }
            return false;
        }

        template<class Invoker>
        void handle(Invoker &invoker) {
            auto now = Counter::now();
            auto dt = now - prev_time;
            if (dt == 0) {
                return;
            }

            prev_time = now;

            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (item.handler == nullptr) {
                    continue;
                }

                auto tm = item.time - dt;
                if (tm == 0 || tm > item.time) {
                    auto clone = item;
                    item.handler = nullptr;
                    invoker(clone);
                } else {
                    item.time = tm;
                }
            }
        }

        template<typename... Args>
        void clear(Args... args) {
            item_type itm(0, args...);
            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (itm.match(item)) {
                    item.handler = nullptr;
                }
            }
        }

        void clear() {
            for (size_t i = 0; i < Capacity; i++) {
                this->items[i].handler = nullptr;
            }
        }

    private:
        counter_type prev_time;
        item_type items[Capacity];
    };

    template<class Token>
    struct function_callback {
        using type = void (*)(Token);

        template<class T>
        static void invoke(const T &item) {
            item.handler(item.token);
        }
    };

    template<>
    struct function_callback<void> {
        using type = void (*)();

        template<class T>
        static void invoke(const T &item) {
            item.handler();
        }
    };

    template<class Counter, size_t Capacity, class Token>
    class function_scheduler : public base_scheduler<Counter, Capacity, typename function_callback<Token>::type, Token> {
    public:
        using TF = function_callback<Token>;
        using base_type = base_scheduler<Counter, Capacity, typename function_callback<Token>::type, Token>;
        using item = typename base_type::item_type;

        void handle() {
            this->base_type::template handle<function_scheduler>(*this);
        }

        void operator()(const item &item) {
            TF::invoke(item);
        }
    };

    template<class Class, class Token>
    struct method_callback {
        using type = void (Class::*)(Token);

        template<class T>
        static void invoke(Class *c, const T &item) {
            (c->*item.handler)(item.token);
        }
    };

    template<class Class>
    struct method_callback<Class, void> {
        using type = void (Class::*)();

        template<class T>
        static void invoke(Class *c, const T &item) {
            (c->*item.handler)();
        }
    };

    template<class T, class Counter, size_t Capacity, class Token>
    class method_scheduler : public base_scheduler<Counter, Capacity, typename method_callback<T, Token>::type, Token> {
    public:
        using timeout_method_type = method_callback<T, Token>;
        using base_type = base_scheduler<Counter, Capacity, typename method_callback<T, Token>::type, Token>;
        using item_type = typename base_type::item_type;

        explicit method_scheduler(T *obj) : instance(obj) {
        }

        void handle() {
            this->base_type::template handle<method_scheduler>(*this);
        }

        void operator()(const item_type &item) {
            timeout_method_type::invoke(instance, item);
        }

    private:
        T *instance;
    };
}}

#endif

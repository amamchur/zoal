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

        schedule_item()
                : time(0), handler(nullptr) {
        }

        schedule_item(const schedule_item &item)
                : time(item.time), handler(item.handler), Token(item.token) {
        }

        schedule_item(CounterValueType tm, Callback c)
                : time(tm), handler(c) {
        }

        schedule_item(CounterValueType tm, Callback c, Token t)
                : time(tm), handler(c), Token(t) {
        }
    };

    template<class CounterValueType, class Callback>
    class schedule_item<CounterValueType, Callback, void> {
    public:
        CounterValueType time;
        Callback handler;

        schedule_item() : time(0), handler(nullptr) {
        }

        schedule_item(const schedule_item &item)
                : time(item.time), handler(item.handler) {
        }

        schedule_item(CounterValueType tm, Callback c)
                : time(tm), handler(c) {
        }
    };

    template<class counter, size_t Capacity, class Callback, class Token>
    class base_scheduler {
    public:
        using token_type = Token;
        using counter_type = typename counter::value_type;
        using item_type = schedule_item<counter_type, Callback, token_type>;

        base_scheduler() : prevTime(0), size(0) {
        }

        template<typename... Args>
        bool schedule(counter_type dt, Args... args) {
            if (size + 1 >= Capacity) {
                return false;
            }

            items[size++] = item_type(dt, args...);
            return true;
        }

        template<class Invoker>
        void handle(Invoker &invoker) {
            auto now = counter::now();
            auto dt = now - prevTime;
            if (dt == 0) {
                return;
            }

            prevTime = now;

            for (intptr_t i = (intptr_t) this->size - 1; i >= 0; i--) {
                auto &item = this->items[i];
                auto tm = item.time - dt;

                if (tm == 0 || tm > item.time) {
                    invoker(item);
                    this->remove(i);
                } else {
                    item.time = tm;
                }
            }
        }

        void clear() {
            size = 0;
        }

    protected:
        counter_type prevTime;
        size_t size;
        item_type items[Capacity];

        void remove(intptr_t index) {
            intptr_t last = (intptr_t) size - 1;
            for (intptr_t i = index; i < last; i++) {
                items[i] = items[i + 1];
            }

            size--;
        }
    };

    template<class Token>
    struct timeout_function {
        template<class T>
        static void invoke(const T &item) {
            item.handler(item.token);
        }
    };

    template<>
    struct timeout_function<void> {
        template<class T>
        static void invoke(const T &item) {
            item.handler();
        }
    };

    template<class counter, size_t Capacity, class Token>
    class function_scheduler : public base_scheduler<counter, Capacity, typename timeout_function<Token>::F, Token> {
    public:
        using TF = timeout_function<Token>;
        using base_type = base_scheduler<counter, Capacity, typename timeout_function<Token>::F, Token>;
        using item = typename base_type::item_type;

        void handle() {
            this->base_type::template handle<function_scheduler>(*this);
        }

        void operator()(const item &item) {
            TF::invoke(item);
        }
    };

    template<class Class, class Token>
    struct timeout_method {
        template<class T>
        static void invoke(Class *c, const T &item) {
            (c->*item.handler)(item.token);
        }
    };

    template<class Class>
    struct timeout_method<Class, void> {
        using method_type = void (Class::*)();

        template<class T>
        static void invoke(Class *c, const T &item) {
            (c->*item.handler)();
        }
    };

    template<class T, class Counter, size_t Capacity, class Token>
    class method_scheduler : public base_scheduler<Counter, Capacity, typename timeout_method<T, Token>::method_type, Token> {
    public:
        using timeout_method_type = timeout_method<T, Token>;
        using base_type = base_scheduler<Counter, Capacity, typename timeout_method<T, Token>::method_type, Token>;
        using item = typename base_type::item_type;

        method_scheduler(T *obj) : instance(obj) {
        }

        void handle() {
            this->base_type::template handle<method_scheduler>(*this);
        }

        void operator()(const item &item) {
            timeout_method_type::invoke(instance, item);
        }

    private:
        T *instance;
    };
}}

#endif

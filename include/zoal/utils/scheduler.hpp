#ifndef ZOAL_UTILS_SCHEDULER_HPP
#define ZOAL_UTILS_SCHEDULER_HPP

#include "../func/function.hpp"

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace utils {
    template<class TicksType, class Handler, class Token, class Id>
    class function_schedule_item {
    public:
        TicksType time;
        Handler handler;
        Id id;
        Token token;

        function_schedule_item() = default;

        function_schedule_item(Id idn, TicksType dt, Handler h)
            : time(dt)
            , handler(h)
            , id(idn) {}

        function_schedule_item(Id idn, TicksType dt, Handler h, Token t)
            : time(dt)
            , handler(h)
            , id(idn)
            , token(t) {}
    };

    template<class TicksType, size_t Capacity, class Token, class Id = int>
    class function_scheduler {
    public:
        using ticks_type = TicksType;
        using id_type = Id;
        using token_type = Token;
        using handler_type = void (*)(Token);
        using item_type = function_schedule_item<TicksType, handler_type, token_type, Id>;

        function_scheduler() noexcept {
            memset(items, 0, sizeof(items));
        }

        template<typename... Args>
        bool schedule(id_type id, ticks_type dt, handler_type handler, Args... args) {
            auto item = find_free_item();
            if (item != nullptr) {
                *item = item_type(id, dt, handler, args...);
            }
            return item != nullptr;
        }

        template<typename... Args>
        bool schedule(id_type id, ticks_type dt, void (*handler)(), Args... args) {
            return schedule(id, dt, reinterpret_cast<void (*)(Token)>(handler), args...);
        }

        void handle(ticks_type now) {
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
                    clone.handler(clone.token);
                } else {
                    item.time = tm;
                }
            }
        }

        void clear() {
            for (size_t i = 0; i < Capacity; i++) {
                this->items[i].handler = nullptr;
            }
        }

        void remove(id_type id) {
            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (item.id == id) {
                    this->items[i].handler = nullptr;
                }
            }
        }

    private:
        item_type *find_free_item() {
            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (!item.handler) {
                    return &item;
                }
            }
            return nullptr;
        }

        TicksType prev_time;
        item_type items[Capacity];
    };

    template<class TickType, class Handler, class Id>
    class lambda_schedule_item {
    public:
        TickType time{0};
        Handler handler;
        Id id{0};
    };

    template<class TicksType, size_t Capacity, size_t ClosureBankSize = sizeof(void *) * 4, class Id = int>
    class lambda_scheduler {
    public:
        using ticks_type = TicksType;
        using id_type = Id;
        using handler_type = zoal::func::function<ClosureBankSize, void>;
        using item_type = lambda_schedule_item<ticks_type, handler_type, id_type>;

        lambda_scheduler() noexcept {
            clear();
        }

        template<class Handler>
        bool schedule(id_type id, ticks_type dt, Handler h) {
            auto item = find_free_item();
            if (item != nullptr) {
                item->id = id;
                item->time = dt;
                item->handler.template assign(h);
            }
            return item != nullptr;
        }

        void handle(ticks_type now) {
            auto dt = now - prev_time;
            if (dt == 0) {
                return;
            }

            prev_time = now;

            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (!item.handler) {
                    continue;
                }

                auto tm = item.time - dt;
                if (tm == 0 || tm > item.time) {
                    item_type clone = item;
                    item.handler.reset();
                    clone.handler();
                } else {
                    item.time = tm;
                }
            }
        }

        void clear() {
            for (size_t i = 0; i < Capacity; i++) {
                this->items[i].handler.reset();
            }
        }

        void remove(id_type id) {
            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (item.id == id) {
                    this->items[i].handler.reset();
                }
            }
        }

    private:
        item_type *find_free_item() {
            for (size_t i = 0; i < Capacity; i++) {
                auto &item = this->items[i];
                if (!item.handler) {
                    return &item;
                }
            }
            return nullptr;
        }

        ticks_type prev_time{0};
        item_type items[Capacity];
    };
}}

#endif

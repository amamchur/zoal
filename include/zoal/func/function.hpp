#ifndef ZOAL_FUNC_FUNCTION_HPP
#define ZOAL_FUNC_FUNCTION_HPP

#include "../utils/new.hpp"

#include <string.h>

namespace zoal { namespace func {
    template<class L, class Ret, class... Args>
    class lambda_container {
    public:
        explicit lambda_container(L l) noexcept
            : lambda(l) {}

        static Ret call_lambda(void *obj, Args... args) {
            return reinterpret_cast<lambda_container *>(obj)->lambda(args...);
        }

    private:
        L lambda;
    };

    template<class L, class... Args>
    class lambda_container<L, void, Args...> {
    public:
        explicit lambda_container(L l) noexcept
            : lambda(l) {}

        static void call_lambda(void *obj, Args... args) {
            reinterpret_cast<lambda_container *>(obj)->lambda(args...);
        }

    private:
        L lambda;
    };

    template<size_t ClosureBankSize, class Ret, class... Args>
    class base_function {
    public:
        template<class L>
        void assign(L l) {
            using inv = lambda_container<L, Ret, Args...>;
            static constexpr bool has_space = sizeof(inv) < ClosureBankSize;
            static_assert(has_space, "Handler size is too big. Please increase closure bank size.");

            new (mem_bank) inv(l);
            fn = inv::call_lambda;
        }

        void reset() {
            fn = nullptr;
        }

        operator bool() const { // NOLINT(google-explicit-constructor)
            return this->fn != nullptr;
        }

    protected:
        Ret (*fn)(void *, Args...){nullptr};
        uint8_t mem_bank[ClosureBankSize]{0};
    };

    template<size_t ClosureBankSize, class Ret, class... Args>
    class function : public base_function<ClosureBankSize, Ret, Args...> {
    public:
        function() = default;

        function(const function &fn) {
            *this = fn;
        }

        template<class T>
        explicit function(T t) {
            this->assign(t);
        }

        function &operator=(const function &fn) {
            if (&fn == this) {
                return *this;
            }

            memcpy(this->mem_bank, fn.mem_bank, ClosureBankSize);
            this->fn = fn.fn;
            return *this;
        }

        template<class T>
        function &operator=(T t) {
            this->assign(t);
            return *this;
        }

        Ret operator()(Args... args) {
            return this->fn(this->mem_bank, args...);
        }
    };

    template<size_t ClosureBankSize, class... Args>
    class function<ClosureBankSize, void, Args...> : public base_function<ClosureBankSize, void, Args...> {
    public:
        function() = default;

        function(const function &fn) {
            *this = fn;
        }

        template<class T>
        explicit function(T t) {
            this->assign(t);
        }

        function &operator=(const function &fn) {
            if (&fn == this) {
                return *this;
            }

            memcpy(this->mem_bank, fn.mem_bank, ClosureBankSize);
            this->fn = fn.fn;
            return *this;
        }

        template<class T>
        function &operator=(T t) {
            this->assign(t);
            return *this;
        }

        void operator()(Args... args) {
            this->fn(this->mem_bank, args...);
        }
    };
}}

#endif

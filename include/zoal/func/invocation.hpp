#ifndef ZOAL_FUNC_INVOCATION_HPP
#define ZOAL_FUNC_INVOCATION_HPP

namespace zoal { namespace func {
    template<class Ret, class... Args>
    class abstract_invocation {
    public:
//        virtual ~abstract_invocation() = default;
        virtual Ret operator()(Args... args) = 0;
    };

    template<class L, class Ret, class... Args>
    class lambda_invocation : public abstract_invocation<Ret, Args...> {
    public:
        explicit lambda_invocation(L l)
            : lambda(l) {}

        Ret operator()(Args... args) override {
            return lambda(args...);
        }

        L lambda;
    };

    template<class L, class... Args>
    class lambda_invocation<L, void, Args...> : public abstract_invocation<void, Args...> {
    public:
        explicit lambda_invocation(L l)
            : lambda(l) {}

        void operator()(Args... args) override {
            lambda(args...);
        }

        L lambda;
    };

    template<class Ret, class... Args>
    class base_invoker {
    public:
        using invocation_type = abstract_invocation<Ret, Args...>;

        Ret operator()(Args... args) {
            return (*invocation_)(args...);
        }

        void reset() {
            if (invocation_ != nullptr) {
                invocation_->~abstract_invocation<void, Args...>();
                invocation_ = nullptr;
            }
        }

    protected:
        invocation_type *invocation_{nullptr};
    };

    template<class... Args>
    class base_invoker<void, Args...> {
    public:
        using invocation_type = abstract_invocation<void, Args...>;

        void operator()(Args... args) {
            (*invocation_)(args...);
        }

        void reset() {
            if (invocation_ != nullptr) {
                invocation_->~abstract_invocation<void, Args...>();
                invocation_ = nullptr;
            }
        }

    protected:
        invocation_type *invocation_{nullptr};
    };
}}

#endif

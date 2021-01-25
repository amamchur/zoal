#include <iostream>

template<class Ret, class... Args>
class abstract_invocation {
public:
    virtual Ret operator()(Args ...args) = 0;
};

template<class T, class Ret, class... Args>
class method_invocation : public abstract_invocation<Ret, Args...> {
public:
    method_invocation(T *obj, Ret (T::*method)(Args ...)) : instance(obj), callback(method) {}

    Ret operator()(Args ...args) override {
        return (*instance.*callback)(args...);
    }

    T *instance;

    Ret (T::*callback)(Args ...);
};

template<class T, class... Args>
class method_invocation<T, void, Args...> : public abstract_invocation<void, Args...> {
public:
    method_invocation(T *obj, void (T::*method)(Args ...)) : instance(obj), callback(method) {}

    void operator()(Args ...args) override {
        (*instance.*callback)(args...);
    }

    T *instance;

    void (T::*callback)(Args ...);
};

template<class T, class Ret, class... Args>
class const_method_invocation : public abstract_invocation<Ret, Args...> {
public:
    const_method_invocation(const T *obj, Ret (T::*method)(Args ...) const) : instance(obj), callback(method) {}

    Ret operator()(Args ...args) override {
        return (*instance.*callback)(args...);
    }

    const T *instance;

    Ret (T::*callback)(Args ...) const;
};

template<class L, class Ret, class... Args>
class lambda_invocation : public abstract_invocation<Ret, Args...> {
public:
    explicit lambda_invocation(L l) : lambda(l) {}

    Ret operator()(Args ...args) override {
        return lambda(args...);
    }

    L lambda;
};

template<class L, class... Args>
class lambda_invocation<L, void, Args...> : public abstract_invocation<void, Args...> {
public:
    explicit lambda_invocation(L l) : lambda(l) {}

    void operator()(Args ...args) override {
        lambda(args...);
    }

    L lambda;
};

template<class Ret, class... Args>
class base_invoker {
public:
protected:
    abstract_invocation<Ret, Args...> *invocation{nullptr};
};

template<class... Args>
class base_invoker<void, Args...> {
public:
protected:
    abstract_invocation<void, Args...> *invocation{nullptr};
};

template<size_t ClosureBankSize, class Ret, class... Args>
class manager : public base_invoker<Ret, Args...> {
public:
    static constexpr size_t bank_size = ClosureBankSize;
    static constexpr size_t bank_count = 2;

    void *get_mem_bank() {
        return mem_banks[active_bank];
    }

    template<class T>
    void create_handler(T *obj, Ret (T::*method)(Args ...)) {
        using invocation_type = method_invocation<T, Ret, Args...>;
        static constexpr bool has_spaces = sizeof(invocation_type) < bank_size;
        static_assert(has_spaces, "Handler size is too big. Please increase closure bank size.");

        auto instance = new(get_mem_bank()) invocation_type(obj, method);
        this->invocation = instance;
    }

    template<class T>
    void create_handler(T *obj, Ret (T::*method)(Args ...) const) {
        using invocation_type = const_method_invocation<T, Ret, Args...>;
        static constexpr bool has_spaces = sizeof(invocation_type) < bank_size;
        static_assert(has_spaces, "Handler size is too big. Please increase closure bank size.");

        auto instance = new(get_mem_bank()) invocation_type(obj, method);
        this->invocation = instance;
    }

    template<class L>
    void create_handler(L l) {
        using invocation_type = lambda_invocation<L, Ret, Args...>;
        static constexpr bool has_spaces = sizeof(invocation_type) < bank_size;
        static_assert(has_spaces, "Handler size is too big. Please increase closure bank size.");

        auto instance = new(get_mem_bank()) invocation_type(l);
        this->invocation = instance;
    }

    void invoke(Args ...args) {
        auto inv = this->invocation;
        active_bank = (active_bank + 1) % bank_count;
        this->invocation = nullptr;
        (*inv)(args...);
    }

    bool has_invocation() const {
        return this->invocation != nullptr;
    }

private:
    size_t active_bank{0};
    uint8_t mem_banks[bank_count][bank_size]{0};
};

using manager_type = manager<96, int>;
manager_type mng;

template<class M>
class mng_client {
public:
    explicit mng_client(M &m) : manager(m) {}

    mng_client(const mng_client &) = delete;

    void start(int number, int &b) {
        auto l3 = [&b, number]() {
            b++;
            std::cout << "callback l3: " << number * b << std::endl;
            return 0;
        };

        auto l2 = [this, l3, &b, number]() {
            b++;
            std::cout << "callback l2: " << number * b << std::endl;
            this->manager.create_handler(l3);
            return 0;
        };

        auto l1 = [this, l2, &b, number]() {
            b++;
            std::cout << "callback l1: " << number * b << std::endl;
            this->manager.create_handler(l2);
            return 0;
        };
        manager.create_handler(l1);
    }

    M &manager;
};

int main() {
    mng_client<manager_type> client(mng);

    int b;

    client.start(5, b);
    while (mng.has_invocation()) {
        mng.invoke();
    }

    std::cout << b << std::endl;

    return 0;
}

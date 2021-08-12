#ifndef ZOAL_FUNC_FUNCTION_HPP
#define ZOAL_FUNC_FUNCTION_HPP

#include "../utils/new.hpp"
#include "./invocation.hpp"

#include <string.h>

namespace zoal { namespace func {
    template<size_t ClosureBankSize, class Ret, class... Args>
    class function : public base_invoker<Ret, Args...> {
    public:
        using invocation_type = typename base_invoker<Ret, Args...>::invocation_type;
        using result_type = Ret;

        function() = default;

        function(const function &fn) {
            *this = fn;
        }

        template<class T>
        explicit function(T t) {
            assign(t);
        }

        function &operator=(const function &fn) {
            if (&fn == this) {
                return *this;
            }

            memcpy(mem_bank, fn.mem_bank, ClosureBankSize);
            uintptr_t d = reinterpret_cast<uint8_t *>(fn.invocation_) - fn.mem_bank;
            this->invocation_ = reinterpret_cast<invocation_type *>(this->mem_bank + d);
            return *this;
        }

        template<class T>
        function &operator=(T t) {
            assign(t);
            return *this;
        }

        operator bool() const { // NOLINT(google-explicit-constructor)
            return this->invocation_ != nullptr;
        }

        template<class L>
        void assign(L l) {
            using inv_type = lambda_invocation<L, Ret, Args...>;
            static constexpr bool has_space = sizeof(inv_type) < ClosureBankSize;
            static_assert(has_space, "Handler size is too big. Please increase closure bank size.");

            this->reset();

            auto instance = new (mem_bank) inv_type(l);
            this->invocation_ = instance;
        }

    private:
        uint8_t mem_bank[ClosureBankSize]{0};
    };
}}

#endif

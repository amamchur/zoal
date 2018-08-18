#ifndef ZOAL_VOLATILE_VALUE_READER_HPP
#define ZOAL_VOLATILE_VALUE_READER_HPP

#include <stddef.h>
#include "interrupts.hpp"

namespace zoal { namespace utils {
    template<class T, const volatile T *Pointer, bool Blocking>
    class base_volatile_value_reader {
    };

    template<class T, const volatile T *Pointer>
    class base_volatile_value_reader<T, Pointer, true> {
    public:
        base_volatile_value_reader() : enableInterrupts(false) {
        }

    private:
        zoal::utils::interrupts enableInterrupts;
    };

    template<class T, const volatile T *Pointer>
    class base_volatile_value_reader<T, Pointer, false> {
    };

    template<class T, const volatile T *Pointer>
    class need_blocking {
    public:
#ifdef __CORTEX_M
        static constexpr bool alignment = 4;
        static constexpr size_t atomic_read_size = 4;
#else
        static constexpr bool alignment = 1;
        static constexpr size_t atomic_read_size = 1;
#endif
        static constexpr bool aligned = ((size_t) Pointer % alignment) == 0;
        static constexpr bool value = sizeof(T) > atomic_read_size;
    };

    template<class T, const volatile T *Pointer>
    class volatile_value_reader : public base_volatile_value_reader<T, Pointer, need_blocking<T, Pointer>::value> {
    public:
        inline operator T() const {
            return *Pointer;
        }
    };
}}

#endif

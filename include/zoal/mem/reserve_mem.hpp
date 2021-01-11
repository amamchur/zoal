#ifndef ZOAL_MEM_STATIC_BUFFER_HPP
#define ZOAL_MEM_STATIC_BUFFER_HPP

#include <stdint.h>

namespace zoal { namespace mem {
    template<size_t Size = 1>
    class reserve_buffer {
    protected:
        uint8_t buffer_ext_[Size];
    };

    template<class Base, size_t Count = 1, class T = uint8_t>
    class reserve_mem : public reserve_buffer<Count * sizeof(T)>, public Base {
    public:
        reserve_mem()
            : Base(this->buffer_ext_, sizeof(this->buffer_ext_)){};

        reserve_mem(const reserve_mem &) = delete;

        template<class... Args>
        explicit reserve_mem(Args... args)
            : Base(this->buffer_ext_, sizeof(this->buffer_ext_), args...) {}
    };
}}

#endif

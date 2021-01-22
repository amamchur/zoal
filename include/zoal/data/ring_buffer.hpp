#ifndef ZOAL_DATA_RING_BUFFER_EXT_HPP
#define ZOAL_DATA_RING_BUFFER_EXT_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace data {
    template<class T>
    class base_ring_buffer {
    public:
        using value_type = T;
        using self_type = base_ring_buffer<T>;

        bool full() const {
            auto next = head_ + 1;
            if (next >= size_) {
                next -= size_;
            }

            return next == tail_;
        }

        inline bool empty() const {
            return head_ == tail_;
        }

        bool push_back(T value) {
            auto next = head_ + 1;
            if (next >= size_) {
                next -= size_;
            }

            if (next == tail_) {
                return false;
            }

            buffer_[head_++] = value;
            head_ = next;

            return true;
        }

        bool pop_front(T &value) {
            if (empty()) {
                return false;
            }

            value = buffer_[tail_++];
            if (tail_ >= size_) {
                tail_ -= size_;
            }

            return true;
        }

    protected:
        explicit base_ring_buffer(size_t buffer_size)
            : size_(buffer_size) {}

    public:
        size_t size_{0};
        size_t head_{0};
        size_t tail_{0};
        T buffer_[1];
    };

    template<class T, size_t Size>
    class ring_buffer : public base_ring_buffer<T> {
    public:
        using base_type = base_ring_buffer<T>;
        using self_type = ring_buffer<T, Size>;

        ring_buffer()
            : base_type(Size + 1) {}

    public:
        volatile T extra_space_[Size];
    };
}}

#endif

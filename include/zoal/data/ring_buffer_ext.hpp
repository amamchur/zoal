#ifndef ZOAL_RING_BUFFER_EXT_HPP
#define ZOAL_RING_BUFFER_EXT_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace data {
    template<class T>
    class base_ring_buffer_ext {
    public:
        using value_type = T;
        using self_type = base_ring_buffer_ext<T>;

        bool push_back(T value) {
            auto next = head_ + 1;
            if (next >= size_) {
                next = 0;
            }

            if (next == tail_) {
                return false;
            }

            buffer_[head_] = value;
            head_ = next;

            return true;
        }

        bool pop_front(T &value) {
            if (tail_ == head_) {
                return false;
            }

            value = buffer_[tail_++];

            if (tail_ >= size_) {
                tail_ = 0;
            }

            return true;
        }

        size_t size() const {
            return size_;
        }

    protected:
        explicit base_ring_buffer_ext(size_t buffer_size)
            : size_(buffer_size) {}

    public:
        size_t size_{0};
        volatile size_t head_{0};
        volatile size_t tail_{0};
        volatile T buffer_[0];
    };

    template<class T, size_t MinSize>
    class ring_buffer_ext : public base_ring_buffer_ext<T> {
    public:
        using base_type = base_ring_buffer_ext<T>;
        using self_type = ring_buffer_ext<T, MinSize>;

        ring_buffer_ext()
            : base_type((sizeof(self_type) - sizeof(base_type)) / sizeof(T)) {}

    public:
        volatile T extra_space_[MinSize];
    };

    template<class T, size_t MinSize, class Control>
    class static_blocking_fifo_buffer {
    public:
        using base_type = base_ring_buffer_ext<T>;
        using self_type = static_blocking_fifo_buffer<T, MinSize, Control>;
        using value_type = T;

        static bool push_back(T value) {
            typename Control::scope_lock sl;
            return buffer_.push_back(value);
        }

        static void push_back_blocking(T value) {
            bool done = false;
            while (!done) {
                typename Control::scope_lock sl;
                done = buffer_.push_back(value);
            }

            Control::item_added();
        }

        static bool pop_front(T &value) {
            typename Control::scope_lock sl;
            return buffer_.pop_front(value);
        }

        static T pop_front_blocking() {
            bool done = false;
            typename base_type::value_type value;
            while (!done) {
                typename Control::scope_lock sl;
                done = buffer_.pop_front(value);
            }

            return value;
        }

    private:
        static ring_buffer_ext<T, MinSize> buffer_;
    };

    template<class T, size_t MinSize, class Control>
    ring_buffer_ext<T, MinSize> static_blocking_fifo_buffer<T, MinSize, Control>::buffer_;

    template<class T>
    class null_fifo_buffer {
    public:
        inline static bool push_back(T value) {
            return false;
        }

        inline static void push_back_blocking(T value) {}

        inline static bool pop_front(T &value) {
            return false;
        }

        inline static T pop_front_blocking() {
            return T();
        }
    };
}}

#endif

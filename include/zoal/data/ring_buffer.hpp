#ifndef ZOAL_DATA_RING_BUFFER_HPP
#define ZOAL_DATA_RING_BUFFER_HPP

#include <stddef.h>
#include <stdint.h>

namespace zoal { namespace data {
    template<class T, size_t Size, void (*yield)()>
    class ring_buffer {
    public:
        ring_buffer()
            : head(buffer)
            , tail(buffer) {}

        operator bool() const {
            return head != tail;
        }

        bool empty() const {
            return head == tail;
        }

        int size() const {
            int length = (int)head - (int)tail;
            if (length < 0) {
                length += Size;
            }

            return length;
        }

        T dequeue(bool force = false) {
            while (force && head == tail) yield();

            T data = *tail++;
            if (tail >= buffer + Size) {
                tail = buffer;
            }

            return data;
        }

        void enqueue(T data, bool force = false) {
            volatile T *next = head + 1;
            if (next >= buffer + Size) {
                next = buffer;
            }

            while (force && next == tail) yield();

            *head = data;
            head = next;
        }

        volatile T buffer[Size];
        volatile T *head;
        volatile T *tail;
    };
}}

#endif

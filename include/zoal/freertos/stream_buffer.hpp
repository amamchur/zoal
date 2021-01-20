#ifndef ZOAL_FREERTOS_STREAM_BUFFER_HPP
#define ZOAL_FREERTOS_STREAM_BUFFER_HPP

#include "./types.hpp"

#include <FreeRTOS.h>
#include <stdint.h>
#include <stream_buffer.h>

namespace zoal { namespace freertos {
    template<class Dummy = void>
    class freertos_stream_buffer {
    public:
        template<class T>
        inline size_t send(const T& value, TickType_t ticks_to_wait = 1) {
            return xStreamBufferSend(handle_, &value, sizeof(T), ticks_to_wait);
        }

        inline size_t send(const void *data, size_t size, TickType_t ticks_to_wait = 1) {
            return xStreamBufferSend(handle_, data, size, ticks_to_wait);
        }

        template<class Yield>
        void send_all(const void *data, size_t size, Yield yield) {
            auto ptr = reinterpret_cast<const char *>(data);
            while (true) {
                auto sent = xStreamBufferSend(handle_, data, size, 0);
                size -= sent;
                ptr += sent;
                if (size > 0) {
                    yield();
                } else {
                    break;
                }
            }
        }

        void send_all(const void *data, size_t size) {
            send_all(data, size, [](){});
        }

        template<class T>
        inline size_t send_isr(T value) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            size_t bytes = xStreamBufferSendFromISR(handle_, &value, sizeof(T), &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            return bytes;
        }

        inline size_t send_isr(const void *data, size_t size) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            size_t bytes = xStreamBufferSendFromISR(handle_, data, size, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            return bytes;
        }

        inline size_t receive(void *data, size_t size, TickType_t ticks_to_wait = portMAX_DELAY) {
            return xStreamBufferReceive(handle_, data, size, ticks_to_wait);
        }

        inline size_t receive_isr(void *data, size_t size) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            size_t bytes = xStreamBufferReceiveFromISR(handle_, data, size, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            return bytes;
        }

        StreamBufferHandle_t handle_{nullptr};
    };

    template<freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class stream_buffer : public freertos_stream_buffer<> {
    public:
        explicit stream_buffer(size_t stream_size, size_t trigger_level_bytes = 1) {
            handle_ = xStreamBufferCreate(stream_size, trigger_level_bytes);
        };
    };

    template<>
    class stream_buffer<freertos_allocation_type::static_mem> : public freertos_stream_buffer<> {
    public:
        explicit stream_buffer(void *buffer, size_t buffer_size, size_t trigger_level_bytes = 1) {
            handle_ = xStreamBufferCreateStatic(buffer_size, trigger_level_bytes, reinterpret_cast<uint8_t *>(buffer), &xStreamBufferStruct);
        };

        StaticStreamBuffer_t xStreamBufferStruct{};
    };
}}

#endif

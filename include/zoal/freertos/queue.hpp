#ifndef ZOAL_FREERTOS_QUEUE_HPP
#define ZOAL_FREERTOS_QUEUE_HPP

#include "./types.hpp"

#include <FreeRTOS.h>
#include <queue.h>
#include <zoal/freertos/types.hpp>

namespace zoal { namespace freertos {
    template<class ItemType>
    class freertos_queue {
    public:
        using item_type = ItemType;

        inline bool push(const ItemType &item, TickType_t ticks_to_wait = 1) {
            return xQueueSend(handle_, &item, ticks_to_wait) == pdTRUE;
        }

        inline bool push(ItemType *item, TickType_t ticks_to_wait = 1) {
            return xQueueSend(handle_, item, ticks_to_wait) == pdTRUE;
        }

        inline bool pop(ItemType *item, TickType_t ticks_to_wait = portMAX_DELAY) {
            return xQueueReceive(handle_, item, ticks_to_wait) == pdTRUE;
        }

        inline bool pop(ItemType &item, TickType_t ticks_to_wait = portMAX_DELAY) {
            return xQueueReceive(handle_, &item, ticks_to_wait) == pdTRUE;
        }

        QueueHandle_t handle_{nullptr};
    };

    template<class ItemType, freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class queue : public freertos_queue<ItemType> {
    public:
        using item_type = ItemType;

        queue() = delete;

        explicit queue(size_t length) {
            this->handle_ = xQueueCreate(length, sizeof(ItemType));
        };
    };

    template<class ItemType>
    class queue<ItemType, freertos_allocation_type::static_mem> : public freertos_queue<ItemType> {
    public:
        using item_type = ItemType;

        queue() = delete;

        explicit queue(void *buffer, size_t buffer_size) {
            auto length = buffer_size / sizeof(ItemType);
            this->handle_ = xQueueCreateStatic(length, sizeof(ItemType), reinterpret_cast<uint8_t *>(buffer), &pxQueueBuffer);
        };

    private:
        StaticQueue_t pxQueueBuffer{};
    };
}}

#endif

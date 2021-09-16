#ifndef ZOAL_FREERTOS_MUTEX_HPP
#define ZOAL_FREERTOS_MUTEX_HPP

#include "./types.hpp"
#include "semphr.h"

namespace zoal { namespace freertos {
    template<class Dummy = void>
    class freertos_mutex {
    public:
        inline bool lock(TickType_t ticks_to_wait = portMAX_DELAY) {
            return xSemaphoreTakeRecursive(handle_, ticks_to_wait) == pdTRUE;
        }

        inline void unlock() {
            xSemaphoreGiveRecursive(handle_);
        }

        SemaphoreHandle_t handle_{nullptr};
    };

    template<freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class mutex : public freertos_mutex<void> {
    public:
        inline mutex() {
            handle_ = xSemaphoreCreateRecursiveMutex();
        };
    };

    template<>
    class mutex<freertos_allocation_type::static_mem> : public freertos_mutex<void> {
    public:
        inline mutex() {
            handle_ = xSemaphoreCreateRecursiveMutexStatic(&semaphore_buffer);
        };

        StaticSemaphore_t semaphore_buffer{nullptr};
    };
}}

#endif

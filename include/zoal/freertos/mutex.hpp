#ifndef ZOAL_FREERTOS_MUTEX_HPP
#define ZOAL_FREERTOS_MUTEX_HPP

#include "./types.hpp"
#include "semphr.h"

namespace zoal { namespace freertos {
    template<class Dummy = void>
    class freertos_mutex {
    public:
        bool lock(TickType_t ticks_to_wait = portMAX_DELAY) {
            return xSemaphoreTake(handle_, ticks_to_wait) == pdTRUE;
        }

        void unlock() {
            xSemaphoreGive(handle_);
        }

        SemaphoreHandle_t handle_{nullptr};
    };

    template<freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class mutex : public freertos_mutex<void> {
    public:
        mutex() {
            handle_ = xSemaphoreCreateMutex();
        };
    };

    template<>
    class mutex<freertos_allocation_type::static_mem> : public freertos_mutex<void> {
    public:
        mutex() {
            handle_ = xSemaphoreCreateMutexStatic(&semaphore_buffer);
        };

        StaticSemaphore_t semaphore_buffer{nullptr};
    };
}}

#endif

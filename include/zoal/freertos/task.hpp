#ifndef ZOAL_FREERTOS_TASK_HPP
#define ZOAL_FREERTOS_TASK_HPP

#include "./types.hpp"

#include <FreeRTOS.h>
#include <stddef.h>
#include <task.h>

namespace zoal { namespace freertos {
    template<class Dummy = void>
    class freertos_task {
    public:
        inline void suspend() {
            vTaskSuspend(handle_);
        }

        inline void resume() {
            vTaskResume(handle_);
        }

        inline void priority(UBaseType_t priority) {
            vTaskPrioritySet(handle_, priority);
        }

        inline UBaseType_t priority() const {
            return uxTaskPriorityGet(handle_);
        }

        inline explicit operator TaskHandle_t() const {
            return handle_;
        }

        TaskHandle_t handle_{nullptr};
    };

    template<freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class task : public freertos_task<> {
    public:
        task() = default;

        explicit task(TaskHandle_t handle) {
            this->handle_ = handle;
        }

        explicit task(TaskFunction_t code,
                      const char *name = nullptr,
                      uint16_t stack_depth = configMINIMAL_STACK_SIZE,
                      void *params = nullptr,
                      UBaseType_t priority = 24) {
            start(code, name, stack_depth, params, priority);
        }

        void start(TaskFunction_t code,
                   const char *name = nullptr,
                   uint16_t stack_depth = configMINIMAL_STACK_SIZE,
                   void *params = nullptr,
                   UBaseType_t priority = 24) {
            xTaskCreate(code, name, stack_depth, params, priority, &handle_);
        }
    };

    template<>
    class task<freertos_allocation_type::static_mem> : public freertos_task<> {
    public:
        task() = default;

        explicit task(TaskHandle_t handle) {
            this->handle_ = handle;
        }

        task(void *buffer, size_t buffer_size, TaskFunction_t code, const char *name = nullptr, void *params = nullptr, UBaseType_t priority = 24) {
            start(buffer, buffer_size, code, name, params, priority);
        }

        void start(void *buffer, size_t buffer_size, TaskFunction_t code, const char *name = nullptr, void *params = nullptr, UBaseType_t priority = 24) {
            auto stack_depth = (buffer_size) / sizeof(StackType_t);
            auto stack = reinterpret_cast<StackType_t *>(buffer);
            handle_ = xTaskCreateStatic(code, name, stack_depth, params, priority, stack, &task_buffer);
        }

    private:
        StaticTask_t task_buffer{};
    };
}}

#endif

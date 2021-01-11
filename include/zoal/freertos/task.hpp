#ifndef ZOAL_FREERTOS_TASK_HPP
#define ZOAL_FREERTOS_TASK_HPP

#include "./types.hpp"

#include <FreeRTOS.h>
#include <stdint.h>
#include <task.h>

namespace zoal { namespace freertos {
    template<class Dummy = void>
    class freertos_task {
    public:
        void suspend() {
            if (handle_) {
                vTaskSuspend(handle_);
            }
        }

        void resume() {
            if (handle_) {
                vTaskResume(handle_);
            }
        }

        void priority(UBaseType_t priority) {
            vTaskPrioritySet(handle_, priority);
        }

        UBaseType_t priority() const {
            return uxTaskPriorityGet(handle_);
        }

        explicit operator TaskHandle_t() const {
            return handle_;
        }

        TaskHandle_t handle_{nullptr};
    };

    template<freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class task : public freertos_task<> {
    public:
        task() = default;

        task(const task &) = delete;

        explicit task(TaskFunction_t code,
                      const char *name = nullptr,
                      uint16_t stack_depth = configMINIMAL_STACK_SIZE,
                      void *params = nullptr,
                      UBaseType_t priority = 24) {
            init(code, name, stack_depth, params, priority);
        }

        void init(TaskFunction_t code,
                  const char *name = nullptr,
                  uint16_t stack_depth = configMINIMAL_STACK_SIZE,
                  void *params = nullptr,
                  UBaseType_t priority = 24) {
            xTaskCreate(code, name, stack_depth, params, priority, &handle_);
        }

        static task start(TaskFunction_t code,
                          const char *name = nullptr,
                          uint16_t stack_depth = configMINIMAL_STACK_SIZE,
                          void *params = nullptr,
                          UBaseType_t priority = 24) {
            return task(code, name, stack_depth, params, priority);
        }
    };

    template<>
    class task<freertos_allocation_type::static_mem> : public freertos_task<> {
    public:
        task() = default;

        task(const task &) = delete;

        explicit task(void *buffer, size_t buffer_size, TaskFunction_t code, const char *name = nullptr, void *params = nullptr, UBaseType_t priority = 24) {
            auto stack_depth = (buffer_size) / sizeof(StackType_t);
            auto stack = reinterpret_cast<StackType_t *>(buffer);
            handle_ = xTaskCreateStatic(code, name, stack_depth, params, priority, stack, &task_buffer);
        }

    private:
        StaticTask_t task_buffer{};
    };
}}

#endif

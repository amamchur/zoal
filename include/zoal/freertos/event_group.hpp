#ifndef ZOAL_FREERTOS_EVENT_GROUP_HPP
#define ZOAL_FREERTOS_EVENT_GROUP_HPP

#include "./types.hpp"

#include <FreeRTOS.h>
#include <event_groups.h>
#include <stdint.h>

namespace zoal { namespace freertos {
    template<class Dummy = void>
    class freertos_event_group {
    public:
        inline EventBits_t set(EventBits_t bits) {
            return xEventGroupSetBits(handle_, bits);
        }

        bool set_isr(EventBits_t bits) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            auto xResult = xEventGroupSetBitsFromISR(handle_, bits, &xHigherPriorityTaskWoken);
            if (xResult != pdFAIL) {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }

            return xResult == pdTRUE;
        }

        inline EventBits_t wait(EventBits_t bits, bool clear = true, bool wait_for_all = false, TickType_t ticks_to_wait = portMAX_DELAY) {
            return xEventGroupWaitBits(handle_, bits, static_cast<BaseType_t>(clear), static_cast<BaseType_t>(wait_for_all), ticks_to_wait);
        }

    protected:
        EventGroupHandle_t handle_{nullptr};
    };

    template<freertos_allocation_type Type = freertos_allocation_type::dynamic_mem>
    class event_group : public freertos_event_group<void> {
    public:
        event_group() {
            handle_ = xEventGroupCreate();
        };
    };

    template<>
    class event_group<freertos_allocation_type::static_mem> : public freertos_event_group<void> {
    public:
        event_group() {
            handle_ = xEventGroupCreateStatic(&event_group_buffer);
        };

    protected:
        StaticEventGroup_t event_group_buffer{};
    };
}}

#endif

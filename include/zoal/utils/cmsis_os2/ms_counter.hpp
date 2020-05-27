#ifndef ZOAL_UTILS_MS_COUNTER_CMSIS_OS2_HPP
#define ZOAL_UTILS_MS_COUNTER_CMSIS_OS2_HPP

#include <stddef.h>
#include <stdint.h>
#include "cmsis_os.h"

namespace zoal { namespace utils { namespace cmsis_os2 {
    class ms_counter {
    public:
        using value_type = uint32_t;

        static inline uint32_t now() {
            return osKernelGetTickCount();
        }
    };
}}}

#endif

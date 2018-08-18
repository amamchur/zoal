#ifndef ZOAL_ARCH_CORTEX_SEMIHOSTING_TRANSPORT_HPP
#define ZOAL_ARCH_CORTEX_SEMIHOSTING_TRANSPORT_HPP

#include <stdint.h> /* NOLINT */
#include <stddef.h>

namespace zoal { namespace utils {
    template<size_t BufferSize = 0x100>
    class semihosting_transport {
    public:
        static constexpr size_t MaxStringLength = BufferSize - 1;

        static void write(uint8_t data) {
            if (index >= MaxStringLength) {
                flush();
            }

            buffer[index++] = data;
        }

        template<class F>
        static void write(F &fn) {
            uint8_t data = 0;
            while (fn(data)) {
                write(data);
            }
        }

        static void flush() {
            buffer[index] = 0;
            int value;
            int reason = 0x04;//SEMIHOSTING_SYS_WRITE0;
            void *arg = buffer;
            asm volatile (
            " mov   r0,     %[rsn]  \n"
            " mov   r1,     %[arg]  \n"
            " bkpt  0xAB \n"
            " mov   %[val], r0"
            :[val] "=r"(value) /* Outputs */
            :[rsn] "r"(reason),[arg] "r"(arg)
            : "r0", "r1", "r2", "r3", "ip", "lr", "memory", "cc"
            );

            index = 0;
        }

        static uint8_t buffer[BufferSize];
        static size_t index;
    };

    template<size_t BufferSize>
    uint8_t semihosting_transport<BufferSize>::buffer[BufferSize];

    template<size_t BufferSize>
    size_t semihosting_transport<BufferSize>::index = 0;

}}

#endif

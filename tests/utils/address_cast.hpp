#ifndef ZOAL_ADDRESS_CAST_HPP
#define ZOAL_ADDRESS_CAST_HPP

#include <iomanip>
#include <iostream>
#include <memory.h>
#include <stdint.h>

#define ZOAL_VOLATILE_ADDRESS_CAST(TYPE, ADDRESS) zoal::test::mem<TYPE>::get_ptr(ADDRESS)
#define ZOAL_ADDRESS_CAST(TYPE, ADDRESS) zoal::test::mem<TYPE>::get_ptr(ADDRESS)

namespace zoal { namespace test {
    extern uint8_t avr_mcu_mem[0x200];
    extern uint32_t cortex_periph_mem[0x30000 / sizeof(uint32_t)];

    template<class T>
    class mem {
    public:
        static T *get_ptr(uintptr_t address) {
            if (address < 0x200) {
                return reinterpret_cast<T *>(avr_mcu_mem + address);
            }
            if (address >= 0x40000000 && address < 0x40030000) {
                auto mp = reinterpret_cast<uint8_t *>(cortex_periph_mem);
                auto offset = address - 0x40000000;
                return reinterpret_cast<T *>(mp + offset);
            }
            return reinterpret_cast<T *>(address);
        }

        static uint8_t *avr() {
            return avr_mcu_mem;
        }

        static void clear_mem() {
            memset(avr_mcu_mem, 0, sizeof(avr_mcu_mem));
        }
    };
}}

#endif

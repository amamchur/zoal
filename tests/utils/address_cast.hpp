#ifndef ZOAL_ADDRESS_CAST_HPP
#define ZOAL_ADDRESS_CAST_HPP

#include <iomanip>
#include <iostream>
#include <memory.h>
#include <stdint.h>

#define ZOAL_ADDRESS_CAST(TYPE, ADDRESS) zoal::test::mem<TYPE>::get_ptr(ADDRESS) //reinterpret_cast<volatile TYPE *>(ADDRESS)

namespace zoal { namespace test {
    extern uint8_t avr_mcu_mem[0x200];

    struct cas_print_functor {
        template<class A>
        void operator()() {
            std::cout << "CAS: *" << (void *)A::address << " & ~" << (void *)A::clear << " | " << (void *)A::set << std::endl;
        }
    };

    template<class T>
    class mem {
    public:
        static T *get_ptr(uintptr_t address) {
            if (address < 0x200) {
                return reinterpret_cast<T *>(avr_mcu_mem + address);
            }
            return nullptr;
        }

        static void print(uintptr_t address) {
            size_t size = 0;
            T *ptr = nullptr;
            if (address < 0x200) {
                ptr = reinterpret_cast<T *>(avr_mcu_mem + address);
                size = sizeof(avr_mcu_mem);
            }

            T *e = ptr + size / sizeof(T);
            auto w = sizeof(T) * 4;
            auto rel = address;
            while (ptr < e) {
                std::cout << "0x" << std::setw(w) << std::setfill('0') << std::hex << (uintptr_t)rel << " 0x" << std::setw(w) << std::setfill('0') << std::hex
                          << (uintptr_t)*ptr << std::endl;
                ptr++;
                rel += sizeof(T);
            }
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

#ifndef ZOAL_TESTS_UTILS_MCU_MEMORY_HPP
#define ZOAL_TESTS_UTILS_MCU_MEMORY_HPP

#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

namespace tests { namespace utils {
    typedef struct {
        uint8_t registers[0x20];
        uint8_t registersIO[0x40];
        uint8_t extRegistersIO[0x1A0];
    } ATmegaMemory;

    template<class RegisterType>
    class register_accessor {
    public:
        register_accessor(uint8_t *memory, uintptr_t base, uintptr_t offset)
                : mem(memory), base(base), offset(offset) {
            address = reinterpret_cast<uintptr_t>(mem) + base + offset;
        }

        operator RegisterType() const {
            RegisterType value = getValue();
            read_callback(address, value);
            return value;
        }

        register_accessor &operator|=(RegisterType value) {
            auto oldValue = getValue();
            auto newValue = oldValue | value;
            setValue(newValue);
            write_callback(address, oldValue, newValue);
            return *this;
        }

        register_accessor &operator&=(RegisterType value) {
            auto oldValue = getValue();
            auto newValue = oldValue & value;
            setValue(newValue);
            write_callback(address, oldValue, newValue);
            return *this;
        }

        register_accessor &operator^=(RegisterType value) {
            auto oldValue = getValue();
            auto newValue = oldValue ^value;
            setValue(newValue);
            write_callback(address, oldValue, newValue);
            return *this;
        }

        register_accessor &operator=(RegisterType value) {
            auto oldValue = getValue();
            setValue(value);
            write_callback(address, oldValue, value);
            return *this;
        }

        static void reset_callbacks() {
            read_callback = [](uintptr_t, RegisterType) {};
            write_callback = [](uintptr_t, RegisterType, RegisterType) {};
        }

        static std::function<void(uintptr_t, RegisterType)> read_callback;
        static std::function<void(uintptr_t, RegisterType, RegisterType)> write_callback;
    private:
        RegisterType getValue() const {
            return *reinterpret_cast<const volatile RegisterType *>(address);
        }

        void setValue(RegisterType value) {
            *reinterpret_cast<volatile RegisterType *>(address) = value;
        }

        volatile uint8_t *mem;
        uintptr_t base;
        uintptr_t offset;
        uintptr_t address;
    };

    template<class RegisterType>
    std::function<void(uintptr_t, RegisterType)>
            register_accessor<RegisterType>::read_callback = [](uintptr_t, RegisterType) {};

    template<class RegisterType>
    std::function<void(uintptr_t, RegisterType, RegisterType)>
            register_accessor<RegisterType>::write_callback = [](uintptr_t, RegisterType, RegisterType) {};

    template<class T, uint8_t WordSize = 1>
    class mcu_memory {
    public:
        template<class RegType, uintptr_t base>
        class memory_segment {
        public:
            typedef register_accessor<RegType> Register;

            Register operator[](uintptr_t offset) {
                return Register(reinterpret_cast<uint8_t *>(&memory), base, offset);
            }
        };

        static void clear() {
            std::memset((char *) &memory, 0, sizeof(T));
        }

        static void save(const std::string &name) {
            using namespace std;

            ofstream file(name, ios::out | ios::binary);
            file.write((char *) &memory, sizeof(memory));
        }

        static bool equal(const std::string &name) {
            using namespace std;

            ifstream file(name, ifstream::ate | ifstream::binary);
            if (file.fail()) {
                error = "File does not exists";
                return false;
            }

            ifstream::pos_type fileSize = file.tellg();
            if (fileSize != sizeof(T)) {
                return false;
            }

            error = "";

            uint8_t data[sizeof(T)];
            file.seekg(0, ios_base::beg);
            file.read((char *) data, sizeof(data));

            auto *mp = reinterpret_cast<uint8_t *>(&memory);
            uint8_t *dp = data;
            size_t position = 0;
            stringstream ss;
            ss.fill('0');
            ss << hex << setw(WordSize * 2);

            while (position < sizeof(T)) {
                int rc = memcmp(mp + position, dp + position, WordSize);
                if (rc != 0) {
                    ss << hex << "0x" << position << ": 0x";
                    for (int i = 0; i < WordSize; i++) {
                        ss << (int) mp[position + i];
                    }

                    ss << " != 0x";
                    for (int i = 0; i < WordSize; i++) {
                        ss << (int) dp[position + i];
                    }

                    ss << endl;
                }

                position += WordSize;
            }

            error = ss.str();

            int rc = memcmp((char *) &memory, data, sizeof(data));
            return rc == 0;
        }

        static T memory;
        static std::string error;
    };

    template<class T, uint8_t WordSize>
    T mcu_memory<T, WordSize>::memory;

    template<class T, uint8_t WordSize>
    std::string mcu_memory<T, WordSize>::error;
}}

#endif

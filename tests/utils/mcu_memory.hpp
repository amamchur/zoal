#ifndef ZOAL_TESTS_UTILS_MCU_MEMORY_HPP
#define ZOAL_TESTS_UTILS_MCU_MEMORY_HPP

#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <zoal/mem/mock.hpp>

namespace tests { namespace utils {
    typedef struct {
        uint8_t registers[0x20];
        uint8_t registersIO[0x40];
        uint8_t extRegistersIO[0x1A0];
    } ATmegaMemory;

    template<class RegisterType, uintptr_t Address, uintptr_t Offset>
    class register_accessor : public zoal::mem::mock<RegisterType> {
    public:
        register_accessor(void *memory, ptrdiff_t mock_offset, uintptr_t hwa, uintptr_t hwo) {
            mock_mem = memory;
            hw_address = hwa + hwo;
            mock_address = reinterpret_cast<uintptr_t>(mock_mem) + mock_offset + hw_address;
        }

        operator RegisterType() const {
            RegisterType value = getValue();
            read_callback(hw_address, value);
            return value;
        }

        register_accessor &operator|=(RegisterType value) {
            auto oldValue = getValue();
            auto newValue = oldValue | value;
            setValue(newValue);
            write_callback(hw_address, oldValue, newValue);
            return *this;
        }

        register_accessor &operator&=(RegisterType value) {
            auto oldValue = getValue();
            auto newValue = oldValue & value;
            setValue(newValue);
            write_callback(hw_address, oldValue, newValue);
            return *this;
        }

        register_accessor &operator^=(RegisterType value) {
            auto oldValue = getValue();
            auto newValue = oldValue ^ value;
            setValue(newValue);
            write_callback(hw_address, oldValue, newValue);
            return *this;
        }

        register_accessor &operator=(RegisterType value) {
            auto oldValue = getValue();
            setValue(value);
            write_callback(hw_address, oldValue, value);
            return *this;
        }

        RegisterType *get_ptr() const override {
            return reinterpret_cast<RegisterType *>(mock_address);
        }

        static void reset_callbacks() {
            read_callback = [](uintptr_t, RegisterType) {};
            write_callback = [](uintptr_t, RegisterType, RegisterType) {};
        }

        static std::function<void(uintptr_t, RegisterType)> read_callback;
        static std::function<void(uintptr_t, RegisterType, RegisterType)> write_callback;

    private:
        RegisterType getValue() const {
            return *reinterpret_cast<const volatile RegisterType *>(mock_address);
        }

        void setValue(RegisterType value) {
            *reinterpret_cast<volatile RegisterType *>(mock_address) = value;
        }

        void *mock_mem{nullptr};
        uintptr_t mock_address{0};
        uintptr_t hw_address{0};
    };

    template<class RegisterType, uintptr_t Address, uintptr_t Offset>
    std::function<void(uintptr_t, RegisterType)> register_accessor<RegisterType, Address, Offset>::read_callback = [](uintptr_t, RegisterType) {};

    template<class RegisterType, uintptr_t Address, uintptr_t Offset>
    std::function<void(uintptr_t, RegisterType, RegisterType)> register_accessor<RegisterType, Address, Offset>::write_callback =
        [](uintptr_t, RegisterType, RegisterType) {};

    template<class T, uint8_t WordSize = 1>
    class mcu_memory {
    public:
        void clear() {
            std::memset((char *)&memory, 0, sizeof(T));
        }

        void save(const std::string &name) {
            using namespace std;

            ofstream file(name, ios::out | ios::binary);
            file.write((char *)&memory, sizeof(memory));
        }

        bool equal(const std::string &name) {
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
            file.read((char *)data, sizeof(data));

            auto *mp = reinterpret_cast<uint8_t *>(&memory);
            uint8_t *dp = data;
            size_t position = 0;
            stringstream ss;
            ss.fill('0');
            ss << hex << std::setw(WordSize * 2);

            while (position < sizeof(T)) {
                int rc = memcmp(mp + position, dp + position, WordSize);
                if (rc != 0) {
                    ss << hex << "0x" << position << ": 0x";
                    for (int i = 0; i < WordSize; i++) {
                        ss << (int)mp[position + i];
                    }

                    ss << " != 0x";
                    for (int i = 0; i < WordSize; i++) {
                        ss << (int)dp[position + i];
                    }

                    ss << endl;
                }

                position += WordSize;
            }

            error = ss.str();

            int rc = memcmp((char *)&memory, data, sizeof(data));
            return rc == 0;
        }

        T memory;
        std::string error;
    };
}}

#endif

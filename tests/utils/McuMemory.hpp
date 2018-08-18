#ifndef TESTS_UTILS_MCU_MEMORY_HPP
#define TESTS_UTILS_MCU_MEMORY_HPP

#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

namespace Tests { namespace Utils {
    typedef struct {
        uint8_t registers[0x20];
        uint8_t registersIO[0x40];
        uint8_t extRegistersIO[0x1A0];
    } ATmegaMemory;

    template<class RegType>
    class RegisterAccessor {
    public:
        RegisterAccessor(uint8_t *memory, uintptr_t base, uintptr_t offset)
                : mem(memory), base(base), offset(offset) {
            address = reinterpret_cast<uintptr_t>(mem) + base + offset;
        }

        operator RegType() const {
            RegType value = getValue();
            readCallback(address, value);
            return value;
        }

        RegisterAccessor &operator|=(RegType value) {
            auto oldValue = getValue();
            auto newValue = oldValue | value;
            setValue(newValue);
            writeCallback(address, oldValue, newValue);
            return *this;
        }

        RegisterAccessor &operator&=(RegType value) {
            auto oldValue = getValue();
            auto newValue = oldValue & value;
            setValue(newValue);
            writeCallback(address, oldValue, newValue);
            return *this;
        }

        RegisterAccessor &operator^=(RegType value) {
            auto oldValue = getValue();
            auto newValue = oldValue ^value;
            setValue(newValue);
            writeCallback(address, oldValue, newValue);
            return *this;
        }

        RegisterAccessor &operator=(RegType value) {
            auto oldValue = getValue();
            setValue(value);
            writeCallback(address, oldValue, value);
            return *this;
        }

        static void resetCallbacks() {
            readCallback = [](uintptr_t, RegType) {};
            writeCallback = [](uintptr_t, RegType, RegType) {};
        }

        static std::function<void(uintptr_t, RegType)> readCallback;
        static std::function<void(uintptr_t, RegType, RegType)> writeCallback;
    private:
        RegType getValue() const {
            return *reinterpret_cast<const volatile RegType *>(address);
        }

        void setValue(RegType value) {
            *reinterpret_cast<volatile RegType *>(address) = value;
        }

        void printReadAccess(RegType value) const {
//            using namespace std;
//            cout.fill('0');
//            cout << hex << setw(sizeof(RegType) * 2);
//            cout << "Read : 0x" << (base + offset) << " Value: 0x" << (unsigned) value << std::endl;
        }

        void printWriteAccess(RegType oldValue, RegType newValue) const {
//            using namespace std;
//            cout.fill('0');
//            cout << hex << setw(sizeof(RegType) * 2);
//            cout << "Write: 0x" << (base + offset) << " Value: 0x" << (unsigned) value << std::endl;
        }

        volatile uint8_t *mem;
        uintptr_t base;
        uintptr_t offset;
        uintptr_t address;
    };

    template<class RegType>
    std::function<void(uintptr_t, RegType)>
            RegisterAccessor<RegType>::readCallback = [](uintptr_t, RegType) {};

    template<class RegType>
    std::function<void(uintptr_t, RegType, RegType)>
            RegisterAccessor<RegType>::writeCallback = [](uintptr_t, RegType, RegType) {};

    template<class T, uint8_t wordSize = 1>
    class McuMemory {
    public:
        template<class RegType, uintptr_t base>
        class MemorySegment {
        public:
            typedef RegisterAccessor<RegType> Register;

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
            ss << hex << setw(wordSize * 2);

            while (position < sizeof(T)) {
                int rc = memcmp(mp + position, dp + position, wordSize);
                if (rc != 0) {
                    ss << hex << "0x" << position << ": 0x";
                    for (int i = 0; i < wordSize; i++) {
                        ss << (int) mp[position + i];
                    }

                    ss << " != 0x";
                    for (int i = 0; i < wordSize; i++) {
                        ss << (int) dp[position + i];
                    }

                    ss << endl;
                }

                position += wordSize;
            }

            error = ss.str();

            int rc = memcmp((char *) &memory, data, sizeof(data));
            return rc == 0;
        }

        static T memory;
        static std::string error;
    };

    template<class T, uint8_t wordSize>
    T McuMemory<T, wordSize>::memory;

    template<class T, uint8_t wordSize>
    std::string McuMemory<T, wordSize>::error;
}}

#endif

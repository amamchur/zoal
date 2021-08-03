#ifndef ZOAL_CMD_ARGUMENT_HPP
#define ZOAL_CMD_ARGUMENT_HPP

#include "./type_detector.hpp"

#include <stdint.h>

namespace zoal { namespace misc {
    class cmd_argument {
    public:
        static cmd_argument *from_buffer(void *buffer) noexcept;

        void string(const char *s, const char *e);

        inline const char *string() const {
            return reinterpret_cast<const char *>(buffer);
        }

        void integer(const char *s, const char *e);

        inline int integer() const {
            return *reinterpret_cast<const int *>(buffer);
        }

        template<class T>
        void set(zoal::misc::value_type t, const T &value) {
            type = t;

            auto s = reinterpret_cast<const uint8_t *>(&value);
            auto e = s + sizeof(T);
            auto ptr = reinterpret_cast<uint8_t *>(buffer);
            while (s != e) {
                *ptr++ = *s++;
            }

            length = sizeof(T);
        }

        template<class T>
        T get() {
            T result;
            auto ptr = reinterpret_cast<uint8_t *>(buffer);
            auto s = reinterpret_cast<uint8_t *>(&result);
            auto e = s + sizeof(T);
            while (s != e) {
                *s++ = *ptr++;
            }
        }

        cmd_argument *prepare_next();

        cmd_argument *next();

        zoal::misc::value_type type{zoal::misc::value_type::string};
        size_t length{0};
        uint8_t buffer[0];
    };
}}

#endif

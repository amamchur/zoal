#pragma once

#ifndef ZOAL_PARSER_INTEGER_HPP
#define ZOAL_PARSER_INTEGER_HPP

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "result.hpp"

namespace zoal { namespace parser {
    enum class integer_parse_state {
        start,
        parsing_int,
        parsing_fract,
        done,
        error
    };

    class number {
    public:
        number(uint8_t radix = 10) : radix(radix) {
            reset();
        }

        operator uint8_t() const {
            return static_cast<uint8_t>(integer);
        }

        operator uint16_t() const {
            return static_cast<uint16_t>(integer);
        }

        operator uint32_t() const {
            return static_cast<uint32_t>(integer);
        }

        operator int8_t() const {
            return static_cast<int8_t>(negative ? -integer : integer);
        }

        operator int16_t() const {
            return static_cast<int16_t>(negative ? -integer : integer);
        }

        operator int32_t() const {
            return negative ? -integer : integer;
        }

        operator double() const {
            return (negative ? -integer : integer) + fraction;
        }

        operator float() const {
            return (negative ? -integer : integer) + fraction;
        }

        uint8_t from_ascii(uint8_t ch) {
            if (ch >= 'A' && ch <= 'Z') {
                return static_cast<uint8_t>(ch - 'A' + 10);
            }

            if (ch >= 'a' && ch <= 'z') {
                return static_cast<uint8_t>(ch - 'a' + 10);
            }
            return ch - '0';
        }

        bool valid(uint8_t ch) {
            uint8_t v = from_ascii(ch);
            return v < radix;
        }

        void reset() {
            state = integer_parse_state::start;
            integer = 0;
            fraction = 0.0;
            exp = 1;
            negative = false;
        }

        bool operator()(char ch) {
            switch (state) {
                case integer_parse_state::start:
                    goto start;
                case integer_parse_state::parsing_int:
                    goto take_int;
                case integer_parse_state::parsing_fract:
                    goto take_fact;
                case integer_parse_state::done:
                    goto number_parsed;
                default:
                    goto error;
            }
start:
            if (isspace(ch)) return true;
            if (ch == 0x2D) {
                negative = true;
                state = integer_parse_state::parsing_int;
                return true;
            }
            state = integer_parse_state::parsing_int;
take_int:
            if (valid(ch)) {
                integer *= radix;
                integer += from_ascii(ch);
                return true;
            }
            if (isspace(ch)) goto number_parsed;
            if (ch == '.') {
                state = integer_parse_state::parsing_fract;
                return true;
            }
            goto error;
take_fact:
            if (valid(ch)) {
                exp = exp / radix;
                fraction += from_ascii(ch) * exp;
                return true;
            }
            if (isspace(ch)) goto number_parsed;
error:
            state = integer_parse_state::error;
number_parsed:
            state = integer_parse_state::done;
            return false;
        }

        integer_parse_state state;
        long integer;
        double fraction;
        double exp;
        bool negative;
        uint8_t radix;
    };
}}

#endif

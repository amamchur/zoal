#ifndef ZOAL_PARSE_TYPE_PARSER_HPP
#define ZOAL_PARSE_TYPE_PARSER_HPP

#include "../data/date_time.hpp"

namespace zoal { namespace parse {
    template<class T>
    static T parse_dec_uint(const char *s, const char *e) {
        T result = 0;
        while (s != e) {
            result = result * 10 + (*s++ - '0');
        }
        return result;
    }

    template<class T>
    class type_parser {};

    template<>
    class type_parser<zoal::data::date_time> {
    public:
        // YYYY.MM.DD HH:MM:SS
        static zoal::data::date_time parse(const char *str) {
            zoal::data::date_time dt;
            dt.year = parse_dec_uint<uint16_t>(str, str + 4);
            dt.month = parse_dec_uint<uint8_t>(str + 5, str + 7);
            dt.date = parse_dec_uint<uint8_t>(str + 8, str + 10);
            dt.hours = parse_dec_uint<uint8_t>(str + 11, str + 13);
            dt.minutes = parse_dec_uint<uint8_t>(str + 14, str + 16);
            dt.seconds = parse_dec_uint<uint8_t>(str + 17, str + 19);
            return dt;
        }

        // YYYY.MM.DD
        static zoal::data::date_time parse_date(const char *str) {
            zoal::data::date_time dt;
            dt.year = parse_dec_uint<uint16_t>(str, str + 4);
            dt.month = parse_dec_uint<uint8_t>(str + 5, str + 7);
            dt.date = parse_dec_uint<uint8_t>(str + 8, str + 10);
            return dt;
        }

        // HH:MM:SS
        static zoal::data::date_time parse_time(const char *str) {
            zoal::data::date_time dt;
            dt.hours = parse_dec_uint<uint8_t>(str, str + 2);
            dt.minutes = parse_dec_uint<uint8_t>(str + 3, str + 5);
            dt.seconds = parse_dec_uint<uint8_t>(str + 6, str + 8);
            return dt;
        }
    };
}}

#endif

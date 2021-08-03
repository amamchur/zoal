#ifndef ZOAL_DATA_DATE_TIME_HPP
#define ZOAL_DATA_DATE_TIME_HPP

#include "../data/date_time.hpp"
#include "../io/output_stream.hpp"

#include <stdint.h>

namespace zoal { namespace data {
    // According to international standard ISO 8601, Monday is the first day of the week
    enum day_of_week { monday = 1, tuesday, wednesday, thursday, friday, saturday, sunday };

    class date_time {
    public:
        uint8_t seconds{0};
        uint8_t minutes{0};
        uint8_t hours{0};
        uint8_t date{1};
        day_of_week day{thursday};
        uint8_t month{1};
        uint16_t year{1970};
    };
}}

template<class T>
zoal::io::output_stream<T> &operator<<(zoal::io::output_stream<T> &os, const zoal::data::date_time &dt) {
    char buffer[] = "0000.00.00 00:00:00";
    auto *p = reinterpret_cast<uint8_t *>(buffer);
    os.backward_number_format(p += 4, dt.year, 10);
    os.backward_number_format(p += 3, dt.month, 10);
    os.backward_number_format(p += 3, dt.date, 10);
    os.backward_number_format(p += 3, dt.hours, 10);
    os.backward_number_format(p += 3, dt.minutes, 10);
    os.backward_number_format(p + 3, dt.seconds, 10);
    os.target.send_data(buffer, sizeof(buffer) - 1);
    return os;
}

#endif
